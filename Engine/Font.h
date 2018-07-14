#pragma once

#include <string>

#include "Colors.h"
#include "Graphics.h"
#include "Rect.h"
#include "Surface.h"
#include "Vec2.h"

class Font
{
public:
	Font( Surface* const surface, Color colour, Color chroma = Colors::Magenta );

	void DrawString( const std::string& text, const Vei2& location, Graphics& gfx ) const;

private:
	RectI MapGlyphRect( char c ) const;

private:
	static constexpr int SpriteSheetColumns = 32;
	static constexpr int SpriteSheetRows = 3;
	static constexpr char FirstChar = ' ';
	static constexpr char LastChar = '~';

	Surface* Sprite = nullptr;

	int GlyphWidth;
	int GlyphHeight;

	Color Chroma;
	Color Colour;
};