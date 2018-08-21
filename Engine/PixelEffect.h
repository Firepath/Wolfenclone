#pragma once

#include "Colors.h"

class Graphics;

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

namespace PixelEffect
{
	class Copy : public Effect
	{
	public:
		void Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const override;
	};

	class Chroma : public Effect
	{
	public:
		Chroma( Color chromaColour = Colors::Magenta );

		void Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const override;

	private:
		Color ChromaColour;
	};

	class Inverse : public Effect
	{
	public:
		Inverse( Color chromaColour = Colors::Magenta );

		void Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const override;

	private:
		Color ChromaColour;
	};

	class InverseSubstitution : public Effect
	{
	public:
		InverseSubstitution( Color substituteColour, Color chromaColour = Colors::Magenta );

		void Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const override;

	private:
		Color ChromaColour;
		Color SubstituteColour;
	};

	class InverseSubstituteTransparency : public Effect
	{
	public:
		InverseSubstituteTransparency( Color substituteColour, float opacity, Color chromaColour = Colors::Magenta );

		void Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const override;

	private:
		Color ChromaColour;
		Color SubstituteColour;
		float Opacity;
	};

	class InverseTransparency : public Effect
	{
	public:
		InverseTransparency( float opacity, Color chromaColour = Colors::Magenta );

		void Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const override;

	private:
		Color ChromaColour;
		float Opacity;
	};

	class Substitution : public Effect
	{
	public:
		Substitution( Color substituteColour, Color chromaColour = Colors::Magenta );

		void Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const override;

	private:
		Color ChromaColour;
		Color SubstituteColour;
	};

	class SubstituteTransparency : public Effect
	{
	public:
		SubstituteTransparency( Color substituteColour, float opacity, Color chromaColour = Colors::Magenta );

		void Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const override;

	private:
		Color ChromaColour;
		Color SubstituteColour;
		float Opacity;
	};

	class Transparency : public Effect
	{
	public:
		Transparency( float opacity, Color chromaColour = Colors::Magenta );

		void Process( Color sourceColour, int destX, int destY, Graphics& gfx ) const override;

	private:
		Color ChromaColour;
		float Opacity;
	};
}
