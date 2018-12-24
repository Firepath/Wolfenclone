#include "Decoration.h"
#include "PixelEffect.h"

Decoration::Decoration( const std::string name, const std::string type, const Surface * const texture )
	:
	Fixture( name, type, texture )
{
}

void Decoration::Draw( const RectI area, Graphics & gfx ) const
{
	if ( Texture != nullptr )
	{
		// Centre the sprite width-wise within the cell
		const float widthRatio = (float)area.GetWidth() / (float)Surface::DefaultTextureWidth;
		const int width = (int)std::ceil( (float)Texture->GetWidth() * widthRatio );
		const float halfWidth = widthRatio * (float)Surface::DefaultTextureWidth * 0.5f;

		const int left = area.left + (int)std::ceil( halfWidth - (float)width * 0.5f );

		// Centre the sprite BOTTOM height-wise within the cell
		const float heightRatio = (float)area.GetHeight() / (float)Surface::DefaultTextureHeight;
		const int height = (int)std::ceil( (float)Texture->GetHeight() * heightRatio );

		const int top = area.top - (int)std::ceil( (float)height * 0.5f );

		const RectI decorationArea( Vei2( left, top ), width, height );

		std::unique_ptr<PixelEffect::Effect> effect = std::make_unique<PixelEffect::Chroma>();
		gfx.DrawSprite( decorationArea, *Texture, effect );
	}
}
