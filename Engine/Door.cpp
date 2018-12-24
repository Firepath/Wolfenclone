#include "Door.h"
#include "PixelEffect.h"

Door::Door( const std::string name, const std::string type, const Surface* const texture )
	:
	Fixture( name, type, texture )
{
}

void Door::Draw( const RectI area, Graphics& gfx ) const
{
	if ( Texture != nullptr )
	{
		std::unique_ptr<PixelEffect::Effect> effect = std::make_unique<PixelEffect::Chroma>();
		gfx.DrawSprite( area, *Texture, effect );
	}
}
