#pragma once

#include <memory>
#include <string>

#include "Rect.h"
#include "Colors.h"

class Surface
{
public:
	Surface() = default;
	Surface( const std::string& filename );
	Surface( const int width, const int height );
	Surface( const Surface& source );
	Surface( Surface&& donor ) noexcept;

	Surface& operator=( const Surface& source );
	Surface& operator=( Surface&& donor ) noexcept;

	void PutPixel( const int x, const int y, const Color colour );
	const Color GetPixel( const int x, const int y ) const;
	const int GetWidth() const;
	const int GetHeight() const;
	RectI GetRect() const;

private:
	int Width = 0;
	int Height = 0;
	std::unique_ptr<Color[]> Pixels;
};