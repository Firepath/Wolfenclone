#include <assert.h>
#include "Sound.h"

#pragma comment(lib, "xaudio2.lib")

SoundSystem& SoundSystem::Get()
{
	static SoundSystem instance;
	return instance;
}

SoundSystem::SoundSystem()
{
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nChannels = 2;
	format.nSamplesPerSec = 44100;
	format.nAvgBytesPerSec = 176400;
	format.nBlockAlign = 4;
	format.wBitsPerSample = 16;
	format.cbSize = 0;
	XAudio2Create( &pEngine );
	pEngine->CreateMasteringVoice( &pMaster );

	pEngine->CreateSubmixVoice( &pSubmix, nChannels, format.nSamplesPerSec );
	submixSendDescriptor.Flags = 0;
	submixSendDescriptor.pOutputVoice = pSubmix;
	submixSendList.SendCount = 1u;
	submixSendList.pSends = &submixSendDescriptor;

	for ( unsigned int i = 0; i < nChannels; i++ )
	{
		idleChannelPtrs.push_back( std::make_unique<Channel>( *this ) );
	}

	pSubmix->SetVolume( 0.2f );

	DWORD channelMask;
	pMaster->GetChannelMask( &channelMask );

	X3DAudioInitialize( channelMask, X3DAUDIO_SPEED_OF_SOUND, x3D );

	emitter.ChannelCount = 1;
	emitter.CurveDistanceScaler = FLT_MIN;

	XAUDIO2_VOICE_DETAILS voiceDetails;
	pSubmix->GetVoiceDetails( &voiceDetails );

	FLOAT32* matrix = new FLOAT32[voiceDetails.InputChannels];

	dspSettings.SrcChannelCount = emitter.ChannelCount;
	dspSettings.DstChannelCount = voiceDetails.InputChannels;
	dspSettings.pMatrixCoefficients = matrix;
}

void SoundSystem::Channel::VoiceCallback::OnBufferEnd( void* pBufferContext )
{
	Channel& chan = *(Channel*)pBufferContext;
	chan.Stop();
	chan.pSound->RemoveChannel( chan );
	chan.pSound = nullptr;
	SoundSystem::Get().DeactivateChannel( chan );
}

void SoundSystem::Channel::PlaySoundBuffer( Sound& s, float freqMod )
{
	assert( pSource && !pSound );
	s.AddChannel( *this );
	pSound = &s;
	xaBuffer.pAudioData = s.pData.get();
	xaBuffer.AudioBytes = s.nBytes;
	pSource->SubmitSourceBuffer( &xaBuffer, nullptr );
	pSource->SetFrequencyRatio( freqMod );
	pSource->Start();
}