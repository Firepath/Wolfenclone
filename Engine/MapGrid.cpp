#include "MapGrid.h"

MapGrid::MapGrid( const int width, const int height, const Vei2& location )
	:
	Width( width ),
	Height( height ),
	Location( location )
{
}

void MapGrid::Draw( Graphics& gfx )
{
	gfx.DrawBoxBorder( Location, Vei2( Location.x + Width * MapGrid::CellSize - 1, Location.y + Height * MapGrid::CellSize - 1 ), MapGrid::GridColour );

	for ( int i = 0; i < Width; i++ )
	{
		Vei2 top = Vei2( i * MapGrid::CellSize, 0 ) + Location;
		Vei2 bottom = Vei2( i * MapGrid::CellSize, Height * MapGrid::CellSize - 1 ) + Location;
		gfx.DrawLine( top, bottom, MapGrid::GridColour );
	}

	for ( int j = 0; j < Height; j++ )
	{
		Vei2 left = Vei2( 0, j * MapGrid::CellSize ) + Location;
		Vei2 right = Vei2( Width * MapGrid::CellSize - 1, j * MapGrid::CellSize ) + Location;
		gfx.DrawLine( left, right, MapGrid::GridColour );
	}
}
