#pragma once

#include "Graphics.h"

namespace PixelEffect
{
	class Blend
	{
	public:
		Color operator()( Color colour1, float ratio, Color colour2 ) const
		{
			const float fraction1 = ratio / 100.0f;
			const float fraction2 = 1.0f - fraction1;

			return Color( (int)(fraction1 * colour1.GetR() + fraction2 * colour2.GetR()), (int)(fraction1 * colour1.GetG() + fraction2 * colour2.GetG()), (int)(fraction1 * colour1.GetB() + fraction2 * colour2.GetB()) );
		}
	};

	class Copy
	{
	public:
		void operator()( Color sourceColour, int destX, int destY, Graphics& gfx ) const
		{
			gfx.PutPixel( destX, destY, sourceColour );
		}
	};

	class Transparency
	{
	public:
		Transparency( Color chromaColour, float opacity )
			:
			ChromaColour( chromaColour ),
			Opacity( opacity )
		{
		}

		void operator()( Color sourceColour, int destX, int destY, Graphics& gfx ) const
		{
			if ( sourceColour != ChromaColour )
			{
				gfx.PutPixel( destX, destY, PixelEffect::Blend()(sourceColour, Opacity, gfx.GetPixel( destX, destY )) );
			}
		}

	private:
		Color ChromaColour;
		float Opacity;
	};
}