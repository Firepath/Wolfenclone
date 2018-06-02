#pragma once

#include "Graphics.h"
#include "Vec2.h"

class MapGrid
{
public:
	MapGrid( const int width, const int height, const Vei2& location );

	void Draw( Graphics& gfx );

private:
	static constexpr int CellSize = 20;
	static constexpr Color GridColour = Colors::DarkGray;

	const int Width;
	const int Height;
	Vei2 Location;
};