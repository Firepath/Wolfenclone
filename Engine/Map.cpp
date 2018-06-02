#include "Map.h"

Map::Map( const int width, const int height, const Vec2& location )
	:
	CellSize( Map::DefaultCellSize ),
	Width( width ),
	Height( height ),
	Location( location ),
	Cells( Width * Height )
{
	for ( int j = 0; j < Height; j++ )
	{
		for ( int i = 0; i < Width; i++ )
		{
			Vei2 l( i, j );
			Cell c( l );
			//c.Colour = Colors::White;
			Cells.emplace( l, c );
		}
	}
}

void Map::Click( const Vei2 & clickLocation )
{
	Vec2 gridLocationF = (Vec2)clickLocation - Location;
	Vei2 gridLocation( (int)(gridLocationF.x / CellSize), (int)(gridLocationF.y / CellSize) );
	if ( gridLocation.x < 0 || gridLocation.y < 0 ||
		gridLocation.x >= Width || gridLocation.y >= Height )
	{
		// Not on the grid
		return;
	}

	Cells.at( gridLocation ).Click();
}

void Map::Draw( Graphics& gfx )
{
	for ( int j = 0; j < Height; j++ )
	{
		const int y = (int)Location.y + j * CellSize;
		const int y1 = y + CellSize - 1;
		if ( y1 < 0 )
		{
			continue;
		}
		else if ( y >= Graphics::ScreenHeight )
		{
			break;
		}

		for ( int i = 0; i < Width; i++ )
		{
			const int x = (int)Location.x + i * CellSize;
			const int x1 = x + CellSize - 1;
			if ( x1 < 0 )
			{
				continue;
			}
			else if ( x >= Graphics::ScreenWidth )
			{
				break;
			}

			const Vei2 cellLocation( i, j );
			Cells.at( cellLocation ).Draw( *this, gfx );
		}
	}

	gfx.DrawBoxBorder( (Vei2)Location, Vei2( (int)Location.x + Width * CellSize - 1, (int)Location.y + Height * CellSize - 1 ), Map::GridColour );

	for ( int i = 0; i < Width; i++ )
	{
		Vei2 top = Vei2( i * CellSize, 0 ) + (Vei2)Location;
		Vei2 bottom = Vei2( i * CellSize, Height * CellSize - 1 ) + (Vei2)Location;
		gfx.DrawLine( top, bottom, Map::GridColour );
	}

	for ( int j = 0; j < Height; j++ )
	{
		Vei2 left = Vei2( 0, j * CellSize ) + (Vei2)Location;
		Vei2 right = Vei2( Width * CellSize - 1, j * CellSize ) + (Vei2)Location;
		gfx.DrawLine( left, right, Map::GridColour );
	}
}

void Map::Move( const Vec2 & delta )
{
	Location += delta;
}

void Map::Zoom( const Vec2& zoomLocation, const float zoomLevel )
{
	const Vec2 oldDistance = Location - zoomLocation;
	const Vec2 newDistance = oldDistance * (zoomLevel / ZoomLevel);
	const Vec2 deltaLocation = newDistance - oldDistance;

	ZoomLevel = zoomLevel;
	CellSize = (int)(Map::DefaultCellSize * ZoomLevel);
	Location += deltaLocation;
}
