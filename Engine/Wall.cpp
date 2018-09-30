#include "Wall.h"
#include "PixelEffect.h"

Wall::Wall( const Surface* const texture )
	:
	MapFixture( texture )
{
}

void Wall::Draw( const RectI area, Graphics & gfx ) const
{
	if ( Texture != nullptr )
	{
		std::unique_ptr<PixelEffect::Effect> effect = std::make_unique<PixelEffect::Chroma>();
		gfx.DrawSprite( area, *Texture, effect );
	}
}
