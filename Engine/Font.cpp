#include <assert.h>

#include "Font.h"
#include "SpriteEffect.h"

Font::Font( Surface* const surface, Color colour, Color chroma )
	:
	Sprite( surface ),
	GlyphWidth( Sprite->GetWidth() / Font::SpriteSheetColumns ),
	GlyphHeight( Sprite->GetHeight() / Font::SpriteSheetRows ),
	Chroma( chroma ),
	Colour( colour )
{
	assert( GlyphWidth * Font::SpriteSheetColumns == Sprite->GetWidth() );
	assert( GlyphHeight * Font::SpriteSheetRows == Sprite->GetHeight() );
}

void Font::DrawString( const std::string& text, const Vei2& location, Graphics& gfx ) const
{
	SpriteEffect::Sustitution effect( Chroma, Colour );
	Vei2 currentLocation = location;
	for ( auto c : text )
	{
		if ( c == '\n' )
		{
			currentLocation.y += GlyphHeight;
			currentLocation.x = location.x;
			continue;
		}

		if ( c >= Font::FirstChar + 1 && c <= Font::LastChar )
		{
			gfx.DrawSprite( currentLocation.x, currentLocation.y, MapGlyphRect( c ), Graphics::GetScreenRect(), *Sprite, effect );
		}

		currentLocation.x += GlyphWidth;
	}
}

const int Font::GetGlyphHeight() const
{
	return GlyphHeight;
}

const int Font::GetGlyphWidth() const
{
	return GlyphWidth;
}

RectI Font::MapGlyphRect( char c ) const
{
	assert( c >= Font::FirstChar && c <= Font::LastChar );

	const int glyphIndex = c - Font::FirstChar;

	const int glyphY = glyphIndex / Font::SpriteSheetColumns;
	const int glyphX = glyphIndex % Font::SpriteSheetColumns;

	return RectI( { glyphX * GlyphWidth, glyphY * GlyphHeight }, GlyphWidth, GlyphHeight );
}
