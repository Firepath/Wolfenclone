#pragma once

#include "PixelEffect.h"

void PixelEffect::Copy::Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const
{
	gfx.PutPixel( destX, destY, sourceColour );
}

PixelEffect::Chroma::Chroma( Color chromaColour )
	:
	ChromaColour( chromaColour )
{
}

void PixelEffect::Chroma::Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const
{
	if ( sourceColour != ChromaColour )
	{
		gfx.PutPixel( destX, destY, sourceColour );
	}
}

PixelEffect::Inverse::Inverse( Color chromaColour )
	:
	ChromaColour( chromaColour )
{
}

void PixelEffect::Inverse::Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const
{
	if ( sourceColour != ChromaColour )
	{
		gfx.PutPixel( destX, destY, ColourTransform::Invert()(sourceColour) );
	}
}

PixelEffect::InverseSubstitution::InverseSubstitution( Color substituteColour, Color chromaColour )
	:
	ChromaColour( chromaColour ),
	SubstituteColour( substituteColour )
{
}

void PixelEffect::InverseSubstitution::Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const
{
	if ( sourceColour != ChromaColour )
	{
		gfx.PutPixel( destX, destY, ColourTransform::Invert()(SubstituteColour) );
	}
}

PixelEffect::InverseSubstituteTransparency::InverseSubstituteTransparency( Color substituteColour, float opacity, Color chromaColour )
	:
	ChromaColour( chromaColour ),
	SubstituteColour( substituteColour ),
	Opacity( opacity )
{
}

void PixelEffect::InverseSubstituteTransparency::Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const
{
	if ( sourceColour != ChromaColour )
	{
		gfx.PutPixel( destX, destY, ColourTransform::Blend()(ColourTransform::Invert()(SubstituteColour), Opacity, gfx.GetPixel( destX, destY )) );
	}
}

PixelEffect::InverseTransparency::InverseTransparency( float opacity, Color chromaColour )
	:
	ChromaColour( chromaColour ),
	Opacity( opacity )
{
}

void PixelEffect::InverseTransparency::Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const
{
	if ( sourceColour != ChromaColour )
	{
		gfx.PutPixel( destX, destY, ColourTransform::Blend()(ColourTransform::Invert()(sourceColour), Opacity, gfx.GetPixel( destX, destY )) );
	}
}

PixelEffect::Substitution::Substitution( Color substituteColour, Color chromaColour )
	:
	ChromaColour( chromaColour ),
	SubstituteColour( substituteColour )
{
}

void PixelEffect::Substitution::Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const
{
	if ( sourceColour != ChromaColour )
	{
		gfx.PutPixel( destX, destY, SubstituteColour );
	}
}

PixelEffect::SubstituteTransparency::SubstituteTransparency( Color substituteColour, float opacity, Color chromaColour )
	:
	ChromaColour( chromaColour ),
	SubstituteColour( substituteColour ),
	Opacity( opacity )
{
}

void PixelEffect::SubstituteTransparency::Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const
{
	if ( sourceColour != ChromaColour )
	{
		gfx.PutPixel( destX, destY, ColourTransform::Blend()(SubstituteColour, Opacity, gfx.GetPixel( destX, destY )) );
	}
}

PixelEffect::Transparency::Transparency( float opacity, Color chromaColour )
	:
	ChromaColour( chromaColour ),
	Opacity( opacity )
{
}

void PixelEffect::Transparency::Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const
{
	if ( sourceColour != ChromaColour )
	{
		gfx.PutPixel( destX, destY, ColourTransform::Blend()(sourceColour, Opacity, gfx.GetPixel( destX, destY )) );
	}
}
