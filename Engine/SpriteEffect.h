#pragma once

#include "Colors.h"
#include "Graphics.h"

namespace ColourTransform
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

	class Invert
	{
	public:
		Color operator()( Color sourceColour ) const
		{
			return Color( 255u - sourceColour.GetR(), 255u - sourceColour.GetG(), 255u - sourceColour.GetB() );
		}
	};
}

namespace SpriteEffect
{
	class Copy
	{
	public:
		void operator()( Color sourceColour, int destX, int destY, Graphics& gfx ) const
		{
			gfx.PutPixel( destX, destY, sourceColour );
		}
	};

	class Chroma
	{
	public:
		Chroma( Color chromaColour )
			:
			ChromaColour( chromaColour )
		{
		}

		void operator()( Color sourceColour, int destX, int destY, Graphics& gfx ) const
		{
			if ( sourceColour != ChromaColour )
			{
				gfx.PutPixel( destX, destY, sourceColour );
			}
		}

	private:
		Color ChromaColour;
	};

	class Inverse
	{
	public:
		Inverse( Color chromaColour )
			:
			ChromaColour( chromaColour )
		{
		}

		void operator()( Color sourceColour, int destX, int destY, Graphics& gfx ) const
		{
			if ( sourceColour != ChromaColour )
			{
				gfx.PutPixel( destX, destY, ColourTransform::Invert()( sourceColour ) );
			}
		}

	private:
		Color ChromaColour;
	};

	class InverseSustitution
	{
	public:
		InverseSustitution( Color chromaColour, Color substituteColour )
			:
			ChromaColour( chromaColour ),
			SubstituteColour( substituteColour )
		{
		}

		void operator()( Color sourceColour, int destX, int destY, Graphics& gfx ) const
		{
			if ( sourceColour != ChromaColour )
			{
				gfx.PutPixel( destX, destY, ColourTransform::Invert()( SubstituteColour ) );
			}
		}

	private:
		Color ChromaColour;
		Color SubstituteColour;
	};

	class InverseSubstituteTransparency
	{
	public:
		InverseSubstituteTransparency( Color chromaColour, Color substituteColour, float opacity )
			:
			ChromaColour( chromaColour ),
			SubstituteColour( substituteColour ),
			Opacity( opacity )
		{
		}

		void operator()( Color sourceColour, int destX, int destY, Graphics& gfx ) const
		{
			if ( sourceColour != ChromaColour )
			{
				gfx.PutPixel( destX, destY, ColourTransform::Blend()( ColourTransform::Invert()( SubstituteColour ), Opacity, gfx.GetPixel( destX, destY ) ) );
			}
		}

	private:
		Color ChromaColour;
		Color SubstituteColour;
		float Opacity;
	};

	class InverseTransparency
	{
	public:
		InverseTransparency( Color chromaColour, float opacity )
			:
			ChromaColour( chromaColour ),
			Opacity( opacity )
		{
		}

		void operator()( Color sourceColour, int destX, int destY, Graphics& gfx ) const
		{
			if ( sourceColour != ChromaColour )
			{
				gfx.PutPixel( destX, destY, ColourTransform::Blend()( ColourTransform::Invert()( sourceColour ), Opacity, gfx.GetPixel( destX, destY ) ) );
			}
		}

	private:
		Color ChromaColour;
		float Opacity;
	};

	class Sustitution
	{
	public:
		Sustitution( Color chromaColour, Color substituteColour )
			:
			ChromaColour( chromaColour ),
			SubstituteColour( substituteColour )
		{
		}

		void operator()( Color sourceColour, int destX, int destY, Graphics& gfx ) const
		{
			if ( sourceColour != ChromaColour )
			{
				gfx.PutPixel( destX, destY, SubstituteColour );
			}
		}

	private:
		Color ChromaColour;
		Color SubstituteColour;
	};

	class SubstituteTransparency
	{
	public:
		SubstituteTransparency( Color chromaColour, Color substituteColour, float opacity )
			:
			ChromaColour( chromaColour ),
			SubstituteColour( substituteColour ),
			Opacity( opacity )
		{
		}

		void operator()( Color sourceColour, int destX, int destY, Graphics& gfx ) const
		{
			if ( sourceColour != ChromaColour )
			{
				gfx.PutPixel( destX, destY, ColourTransform::Blend()( SubstituteColour, Opacity, gfx.GetPixel( destX, destY ) ) );
			}
		}

	private:
		Color ChromaColour;
		Color SubstituteColour;
		float Opacity;
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
				gfx.PutPixel( destX, destY, ColourTransform::Blend()( sourceColour, Opacity, gfx.GetPixel( destX, destY ) ) );
			}
		}

	private:
		Color ChromaColour;
		float Opacity;
	};
}
