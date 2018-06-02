#include "MapGrid.h"

MapGrid::MapGrid( const int width, const int height, const Vec2& location )
	:
	Width( width ),
	Height( height ),
	Location( location )
{
}

void MapGrid::Draw( Graphics& gfx )
{
	const int cellSize = GetCellSize();

	gfx.DrawBoxBorder( (Vei2)Location, Vei2( (int)Location.x + Width * cellSize - 1, (int)Location.y + Height * cellSize - 1 ), MapGrid::GridColour );

	for ( int i = 0; i < Width; i++ )
	{
		Vei2 top = Vei2( i * cellSize, 0 ) + (Vei2)Location;
		Vei2 bottom = Vei2( i * cellSize, Height * cellSize - 1 ) + (Vei2)Location;
		gfx.DrawLine( top, bottom, MapGrid::GridColour );
	}

	for ( int j = 0; j < Height; j++ )
	{
		Vei2 left = Vei2( 0, j * cellSize ) + (Vei2)Location;
		Vei2 right = Vei2( Width * cellSize - 1, j * cellSize ) + (Vei2)Location;
		gfx.DrawLine( left, right, MapGrid::GridColour );
	}
}

void MapGrid::Move( const Vec2 & delta )
{
	Location += delta;
}

void MapGrid::Zoom( const Vec2& zoomLocation, const float zoomLevel )
{
	const Vec2 oldDistance = Location - zoomLocation;
	const Vec2 newDistance = oldDistance * (zoomLevel / ZoomLevel);
	const Vec2 deltaLocation = newDistance - oldDistance;

	ZoomLevel = zoomLevel;
	Location += deltaLocation;
}

int MapGrid::GetCellSize() const
{
	return (int)(MapGrid::DefaultCellSize * ZoomLevel);
}
