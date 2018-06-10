#include "Surface.h"
#include "ChiliWin.h"
#include "ChiliException.h"
#include "Graphics.h"

#include <cassert>
#include <codecvt>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>

#define CHILI_GFX_FILE_EXCEPTION( note, exception ) Graphics::FileException( note, exception, _CRT_WIDE(__FILE__), __LINE__ )

Surface::Surface( const std::string & filename )
{
	std::ifstream file;
	file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
	try
	{
		file.open(filename, std::ios::binary);
	}
	catch ( const std::exception& ex )
	{
		std::string message = "Failed to load file '" + filename + "'.";
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		throw CHILI_GFX_FILE_EXCEPTION( converter.from_bytes( message.c_str() ), ex );
	}

	BITMAPFILEHEADER bmpFileHeader;
	file.read( reinterpret_cast<char*>(&bmpFileHeader), sizeof( bmpFileHeader ) );

	BITMAPINFOHEADER bmpInfoHeader;
	file.read( reinterpret_cast<char*>(&bmpInfoHeader), sizeof( bmpInfoHeader ) );

	if ( bmpInfoHeader.biBitCount != 24 && bmpInfoHeader.biBitCount != 32 )
	{
		std::ostringstream message;
		message << "Unsupported bit count.\n\nImage file '" << filename << "' is " << bmpInfoHeader.biBitCount << "-bit not 24-bit or 32-bit.";
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		throw CHILI_GFX_FILE_EXCEPTION( converter.from_bytes( message.str().c_str() ), std::exception() );
	}


	if ( bmpInfoHeader.biCompression != BI_RGB )
	{
		std::ostringstream message;
		message << "Unsupported image compression.\n\nImage file '" << filename << "' is " << bmpInfoHeader.biCompression << " not BI_RGB.";
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		throw CHILI_GFX_FILE_EXCEPTION( converter.from_bytes( message.str().c_str() ), std::exception() );
	}

	bool is32bit = bmpInfoHeader.biBitCount == 32;

	Width = bmpInfoHeader.biWidth;

	int startHeight;
	int endHeight;
	int heightDirection;
	if ( bmpInfoHeader.biHeight < 0 )
	{
		Height = -bmpInfoHeader.biHeight;
		startHeight = 0;
		endHeight = Height;
		heightDirection = 1;
	}
	else
	{
		Height = bmpInfoHeader.biHeight;
		startHeight = Height - 1;
		endHeight = -1;
		heightDirection = -1;
	}

	Pixels = std::make_unique<Color[]>( Width * Height );

	file.seekg( bmpFileHeader.bfOffBits );
	const unsigned int padding = (4 - (Width * 3) % 4) % 4;

	for ( int y = startHeight; y != endHeight; y += heightDirection )
	{
		for ( int x = 0; x < Width; x++ )
		{
			PutPixel( x, y, Color( file.get(), file.get(), file.get() ) );

			if ( is32bit )
			{
				// Skip A
				file.get();
			}
		}

		if ( !is32bit )
		{
			file.seekg( padding, std::ios::cur );
		}
	}
}

Surface::Surface( const int width, const int height )
	:
	Width( width ),
	Height( height ),
	Pixels( std::make_unique<Color[]>( width * height ) )
{
}

Surface::Surface( const Surface & source )
	:
	Surface( source.Width, source.Height )
{
	const int pixelCount = Width * Height;
	for ( int i = 0; i < pixelCount; i++ )
	{
		Pixels[i] = source.Pixels[i];
	}
}

Surface::Surface( Surface && donor ) noexcept
	:
	Width( donor.Width ),
	Height( donor.Height ),
	Pixels( std::make_unique<Color[]>( Width * Height ) )
{
	donor.Width = 0;
	donor.Height = 0;
	const auto begin = donor.Pixels.get();
	std::move( begin, std::next( begin, Width * Height ), stdext::checked_array_iterator<Color*>( Pixels.get(), Width * Height ) );
}

Surface & Surface::operator=( const Surface & source )
{
	if ( &source != this )
	{
		Width = source.Width;
		Height = source.Height;

		Pixels = std::make_unique<Color[]>( Width * Height );

		const int pixelCount = Width * Height;
		for ( int i = 0; i < pixelCount; i++ )
		{
			Pixels[i] = source.Pixels[i];
		}
	}

	return *this;
}

Surface & Surface::operator=( Surface && donor ) noexcept
{
	if ( &donor != this )
	{
		Width = donor.Width;
		Height = donor.Height;

		donor.Width = 0;
		donor.Height = 0;

		Pixels = std::make_unique<Color[]>( Width * Height );
		const auto begin = donor.Pixels.get();
		std::move( begin, std::next( begin, Width * Height ), stdext::checked_array_iterator<Color*>( Pixels.get(), Width * Height ) );
	}

	return *this;
}

void Surface::PutPixel( const int x, const int y, const Color colour )
{
	assert( x >= 0 );
	assert( x < Width );
	assert( y >= 0 );
	assert( y < Height );

	Pixels[y * Width + x] = colour;
}

const Color Surface::GetPixel( const int x, const int y ) const
{
	assert( x >= 0 );
	assert( x < Width );
	assert( y >= 0 );
	assert( y < Height );

	return Pixels[y * Width + x];
}

const int Surface::GetWidth() const
{
	return Width;
}

const int Surface::GetHeight() const
{
	return Height;
}

RectI Surface::GetRect() const
{
	return RectI( 0, Width, 0, Height );
}
