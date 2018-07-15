#pragma once

#include <string>

#include "Colors.h"
#include "Graphics.h"
#include "PixelEffect.h"
#include "Rect.h"
#include "Surface.h"
#include "Vec2.h"

class Font
{
public:
	Font( const Surface& surface, Color colour, Color chroma = Colors::Magenta );
	Font( const Surface& surface, Color colour, size_t height, Color chroma = Colors::Magenta );

	void DrawString( const std::string& text, const Vei2& location, Graphics& gfx ) const
	{
		DrawString( text, location, Graphics::GetScreenClippingRect(), gfx );
	}

	void DrawString( const std::string& text, const Vei2& location, const RectI& clippingRect, Graphics& gfx ) const
	{
		PixelEffect::Sustitution effect( Colour, Chroma );
		DrawString( text, location, clippingRect, effect, gfx );
	}
	
	template<typename E>
	void DrawString( const std::string& text, const Vei2& location, const RectI& clippingRect, E effect, Graphics& gfx ) const
	{
		const float scale = GetScale();
		const int glyphWidth = (int)(GlyphWidth * scale);
		const int glyphHeight = (int)(GlyphHeight * scale);

		Vei2 currentLocation = location;
		for ( auto c : text )
		{
			if ( c == '\n' )
			{
				currentLocation.y += glyphHeight;
				currentLocation.x = location.x;
				continue;
			}

			if ( c >= Font::FirstChar + 1 && c <= Font::LastChar )
			{
				gfx.DrawSprite( RectI(Vei2(currentLocation.x, currentLocation.y), glyphWidth, glyphHeight), MapGlyphRect( c ), clippingRect, Sprite, effect );
			}

			currentLocation.x += glyphWidth;
		}
	}

	const size_t GetHeight() const;
	const size_t GetWidth() const;
	void SetHeight( const size_t height );

private:
	const int GetGlyphHeight() const;
	const int GetGlyphWidth() const;
	const float GetScale() const;
	RectI MapGlyphRect( char c ) const;

private:
	static constexpr int SpriteSheetColumns = 32;
	static constexpr int SpriteSheetRows = 3;
	static constexpr char FirstChar = ' ';
	static constexpr char LastChar = '~';

	const Surface& Sprite;

	int GlyphWidth;
	int GlyphHeight;

	size_t Height;

	Color Chroma;
	Color Colour;
};