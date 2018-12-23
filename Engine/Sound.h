#pragma once

#define NOMINMAX

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <memory>
#include <mutex>
#include <unordered_set>
#include <vector>
#include <wrl\client.h>
#include <x3daudio.h>
#include <xaudio2.h>

#include "ComManager.h"

class SoundSystem
{
public:
	class Error : public std::runtime_error
	{
	public:
		Error( std::string s )
			:
			runtime_error( std::string( "SoundSystem::Error: " ) + s )
		{}
	};
	class FileError : public Error
	{
	public:
		FileError( std::string s )
			:
			Error( std::string( "SoundSystem::FileError: " ) + s )
		{}
	};
public:
	class Channel
	{
	private:
		class VoiceCallback : public IXAudio2VoiceCallback
		{
		public:
			void STDMETHODCALLTYPE OnStreamEnd() override {}
			void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() override {}
			void STDMETHODCALLTYPE OnVoiceProcessingPassStart( UINT32 SamplesRequired ) override {}
			void STDMETHODCALLTYPE OnBufferEnd( void* pBufferContext ) override;
			void STDMETHODCALLTYPE OnBufferStart( void* pBufferContext ) override {}
			void STDMETHODCALLTYPE OnLoopEnd( void* pBufferContext ) override {}
			void STDMETHODCALLTYPE OnVoiceError( void* pBufferContext, HRESULT Error ) override {}
		};
	public:
		Channel( SoundSystem& sys )
		{
			static VoiceCallback vcb;
			ZeroMemory( &xaBuffer, sizeof( xaBuffer ) );
			xaBuffer.pContext = this;
			sys.pEngine->CreateSourceVoice( &pSource, &sys.format, 0u, 2.0f, &vcb, &sys.submixSendList );
		}
		~Channel()
		{
			assert( !pSound );
			if ( pSource )
			{
				pSource->DestroyVoice();
				pSource = nullptr;
			}
		}
		void AdjustVolume( float delta )
		{
			float volume;
			pSource->GetVolume( &volume );
			volume = std::max( 0.0f, volume + delta );
			pSource->SetVolume( volume );
		}
		void PlaySoundBuffer( class Sound& s, float freqMod );
		void Stop()
		{
			assert( pSource && pSound );
			pSource->Stop();
			pSource->FlushSourceBuffers();
		}
	private:
		XAUDIO2_BUFFER xaBuffer;
		IXAudio2SourceVoice* pSource = nullptr;
		class Sound* pSound = nullptr;
	};
public:
	static SoundSystem& Get();
	static WAVEFORMATEX& GetFormat()
	{
		return Get().format;
	}
	void AdjustVolume( float delta )
	{
		std::lock_guard<std::mutex> lock( mutex );
		float volume;
		pSubmix->GetVolume( &volume );
		volume = std::max( 0.0f, volume + delta );
		pSubmix->SetVolume( volume );
	}
	void PlaySoundBuffer( class Sound& s, float freqMod )
	{
		std::lock_guard<std::mutex> lock( mutex );
		if ( idleChannelPtrs.size() > 0 )
		{
			activeChannelPtrs.push_back( std::move( idleChannelPtrs.back() ) );
			idleChannelPtrs.pop_back();
			activeChannelPtrs.back()->PlaySoundBuffer( s, freqMod );
		}
	}
	void Update()
	{
		std::lock_guard<std::mutex> lock( mutex );
		
		emitter.OrientFront = X3DAUDIO_VECTOR{ 0.0f, 0.0f, 1.0f };
		emitter.OrientTop = X3DAUDIO_VECTOR{ 0.0f, 1.0f, 0.0f };
		emitter.Position = X3DAUDIO_VECTOR{ 10.0f, 0.0f, -10.0f };
		emitter.Velocity = X3DAUDIO_VECTOR{ 0.0f, 0.0f, 0.0f };

		listener.OrientFront = X3DAUDIO_VECTOR{ 0.0f, 0.0f, -1.0f };
		listener.OrientTop = X3DAUDIO_VECTOR{ 0.0f, 1.0f, 0.0f };
		listener.Position = X3DAUDIO_VECTOR{ 0.0f, 0.0f, 0.0f };
		listener.Velocity = X3DAUDIO_VECTOR{ 0.0f, 0.0f, 0.0f };

		X3DAudioCalculate( x3D, &listener, &emitter, X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB, &dspSettings );

		// This is supposed to be source voice
		pSubmix->SetOutputMatrix( pMaster, 1, nChannels, dspSettings.pMatrixCoefficients );
		//pSubmix->SetOutputMatrix( pMaster, nChannels, 1, dspSettings.pMatrixCoefficients );

		// source voice set freq ratio for doppler

		//source voice->SetOutputMatrix( pSubmix, 1, 1, &dspSettings.ReverbLevel );
	}
private:
	SoundSystem();
	void DeactivateChannel( Channel& channel )
	{
		std::lock_guard<std::mutex> lock( mutex );
		auto i = std::find_if( activeChannelPtrs.begin(), activeChannelPtrs.end(), [&channel]( const std::unique_ptr<Channel>& pChan ) -> bool
		{
			return &channel == pChan.get();
		} );
		idleChannelPtrs.push_back( std::move( *i ) );
		activeChannelPtrs.erase( i );
	}
private:
	ComManager comMan;
	Microsoft::WRL::ComPtr<IXAudio2> pEngine;
	XAUDIO2_SEND_DESCRIPTOR submixSendDescriptor;
	XAUDIO2_VOICE_SENDS submixSendList;
	IXAudio2MasteringVoice* pMaster = nullptr;
	IXAudio2SubmixVoice* pSubmix = nullptr;
	WAVEFORMATEX format;
	const unsigned int nChannels = 64u;
	std::mutex mutex;
	std::vector<std::unique_ptr<Channel>> idleChannelPtrs;
	std::vector<std::unique_ptr<Channel>> activeChannelPtrs;

	X3DAUDIO_HANDLE x3D;
	X3DAUDIO_LISTENER listener;
	X3DAUDIO_EMITTER emitter = { 0 };
	X3DAUDIO_DSP_SETTINGS dspSettings;
};

class Sound
{
	friend SoundSystem::Channel;
public:
	Sound( const std::wstring& fileName )
	{
		unsigned int fileSize = 0;
		std::unique_ptr<BYTE[]> pFileIn;
		try
		{
			{
				std::ifstream file;
				file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
				try
				{
					file.open( fileName, std::ios_base::binary );
				}
				catch ( std::exception e )
				{
					throw SoundSystem::FileError( std::string( "error opening file: " ) + e.what() );
				}
				{
					int fourcc;
					file.read( reinterpret_cast<char*>(&fourcc), 4 );
					if ( fourcc != 'FFIR' )
					{
						throw SoundSystem::FileError( "bad fourCC" );
					}
				}

				file.read( reinterpret_cast<char*>(&fileSize), 4 );
				fileSize += 8; // entry doesn't include the fourcc or itself
				if ( fileSize <= 44 )
				{
					throw SoundSystem::FileError( "file too small" );
				}

				file.seekg( 0, std::ios::beg );
				pFileIn = std::make_unique<BYTE[]>( fileSize );
				file.read( reinterpret_cast<char*>(pFileIn.get()), fileSize );
			}

			if ( *reinterpret_cast<const int*>(&pFileIn[8]) != 'EVAW' )
			{
				throw SoundSystem::FileError( "format not WAVE" );
			}

			// look for 'fmt ' chunk id
			WAVEFORMATEX format;
			bool bFilledFormat = false;
			for ( unsigned int i = 12; i < fileSize; )
			{
				if ( *reinterpret_cast<const int*>(&pFileIn[i]) == ' tmf' )
				{
					memcpy( &format, &pFileIn[i + 8], sizeof( format ) );
					bFilledFormat = true;
					break;
				}
				// chunk size + size entry size + chunk id entry size + word padding
				i += (*reinterpret_cast<const int*>(&pFileIn[i + 4]) + 9) & 0xFFFFFFFE;
			}
			if ( !bFilledFormat )
			{
				throw SoundSystem::FileError( "fmt chunk not found" );
			}

			// compare format with sound system format
			{
				const WAVEFORMATEX sysFormat = SoundSystem::GetFormat();

				if ( format.nChannels != sysFormat.nChannels )
				{
					throw SoundSystem::FileError( "bad wave format (nChannels)" );
				}
				else if ( format.wBitsPerSample != sysFormat.wBitsPerSample )
				{
					throw SoundSystem::FileError( "bad wave format (wBitsPerSample)" );
				}
				else if ( format.nSamplesPerSec != sysFormat.nSamplesPerSec )
				{
					throw SoundSystem::FileError( "bad wave format (nSamplesPerSec)" );
				}
				else if ( format.wFormatTag != sysFormat.wFormatTag )
				{
					throw SoundSystem::FileError( "bad wave format (wFormatTag)" );
				}
				else if ( format.nBlockAlign != sysFormat.nBlockAlign )
				{
					throw SoundSystem::FileError( "bad wave format (nBlockAlign)" );
				}
				else if ( format.nAvgBytesPerSec != sysFormat.nAvgBytesPerSec )
				{
					throw SoundSystem::FileError( "bad wave format (nAvgBytesPerSec)" );
				}
				else if ( format.wBitsPerSample != sysFormat.wBitsPerSample )
				{
					throw SoundSystem::FileError( "bad wave format (wBitsPerSample)" );
				}
			}

			// look for 'data' chunk id
			bool bFilledData = false;
			for ( unsigned int i = 12; i < fileSize; )
			{
				const int chunkSize = *reinterpret_cast<const int*>(&pFileIn[i + 4]);
				if ( *reinterpret_cast<const int*>(&pFileIn[i]) == 'atad' )
				{
					pData = std::make_unique<BYTE[]>( chunkSize );
					nBytes = chunkSize;
					memcpy( pData.get(), &pFileIn[i + 8], nBytes );

					bFilledData = true;
					break;
				}
				// chunk size + size entry size + chunk id entry size + word padding
				i += (chunkSize + 9) & 0x0FFFFFFFE;
			}
			if ( !bFilledData )
			{
				throw SoundSystem::FileError( "data chunk not found" );
			}
		}
		catch ( SoundSystem::Error )
		{
			throw;
		}
		catch ( std::exception e )
		{
			throw SoundSystem::FileError( e.what() );
		}
	}
	void Play( float freqMod = 1.0f )
	{
		SoundSystem::Get().PlaySoundBuffer( *this, freqMod );
	}
	~Sound()
	{
		{
			std::lock_guard<std::mutex> lock( mutex );
			for ( auto pChannel : activeChannelPtrs )
			{
				pChannel->Stop();
			}
		}

		bool allChannelsDeactivated = false;
		do
		{
			std::lock_guard<std::mutex> lock( mutex );
			allChannelsDeactivated = activeChannelPtrs.size() == 0;
		}
		while ( !allChannelsDeactivated );
	}
private:
	void RemoveChannel( SoundSystem::Channel& channel )
	{
		std::lock_guard<std::mutex> lock( mutex );
		activeChannelPtrs.erase( std::find( activeChannelPtrs.begin(), activeChannelPtrs.end(), &channel ) );
	}
	void AddChannel( SoundSystem::Channel& channel )
	{
		std::lock_guard<std::mutex> lock( mutex );
		activeChannelPtrs.push_back( &channel );
	}
private:
	UINT32 nBytes = 0;
	std::unique_ptr<BYTE[]> pData;
	std::mutex mutex;
	std::vector<SoundSystem::Channel*> activeChannelPtrs;
};