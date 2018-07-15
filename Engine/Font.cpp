#include <assert.h>

#include "Font.h"

Font::Font( const Surface& surface, Color colour, Color chroma )
	:
	Sprite( surface ),
	GlyphWidth( Sprite.GetWidth() / Font::SpriteSheetColumns ),
	GlyphHeight( Sprite.GetHeight() / Font::SpriteSheetRows ),
	Chroma( chroma ),
	Colour( colour ),
	Height( (size_t)GlyphHeight )
{
	assert( GlyphWidth * Font::SpriteSheetColumns == Sprite.GetWidth() );
	assert( GlyphHeight * Font::SpriteSheetRows == Sprite.GetHeight() );
}

Font::Font( const Surface & surface, Color colour, size_t height, Color chroma )
	:
	Font( surface, colour, chroma )
{
	SetHeight( height );
}

const int Font::GetGlyphHeight() const
{
	return GlyphHeight;
}

const int Font::GetGlyphWidth() const
{
	return GlyphWidth;
}

const size_t Font::GetHeight() const
{
	return Height;
}

void Font::SetHeight( const size_t height )
{
	Height = height;
}

const float Font::GetScale() const
{
	return ((float)Height) / (float)GlyphHeight;
}

RectI Font::MapGlyphRect( char c ) const
{
	assert( c >= Font::FirstChar && c <= Font::LastChar );

	const int glyphIndex = c - Font::FirstChar;

	const int glyphY = glyphIndex / Font::SpriteSheetColumns;
	const int glyphX = glyphIndex % Font::SpriteSheetColumns;

	return RectI( { glyphX * GlyphWidth, glyphY * GlyphHeight }, GlyphWidth, GlyphHeight );
}
