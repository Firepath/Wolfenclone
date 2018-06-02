#pragma once

#include "Graphics.h"
#include "Vec2.h"

class MapGrid
{
public:
	MapGrid( const int width, const int height, const Vec2& location );

	void Draw( Graphics& gfx );
	void Move( const Vec2& delta );
	void Zoom( const Vec2& zoomLocation, const float zoomLevel );

private:
	static constexpr Color GridColour = Colors::DarkGray;
	static constexpr int DefaultCellSize = 40;

	int GetCellSize() const;

	float ZoomLevel = 1.0f;
	const int Width;
	const int Height;
	Vec2 Location;
};