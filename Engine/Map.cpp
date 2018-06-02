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
			Cells.emplace( l, c );
		}
	}
}

void Map::Draw( Graphics& gfx )
{
	for ( int j = 0; j < Height; j++ )
	{
		const int y = (int)(Location.y + j * CellSize);
		const int y1 = y + (int)CellSize - 1;
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
			const int x = (int)(Location.x + i * CellSize);
			const int x1 = x + (int)CellSize - 1;
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

	// Grid border
	Vei2 topLeft = (Vei2)Location;
	Vei2 bottomRight = Vei2( (int)std::ceil( Location.x ) + (int)std::ceil( Width * CellSize ) - 1, (int)std::ceil( Location.y ) + (int)std::ceil( Height * CellSize ) - 1 );
	gfx.DrawBoxBorder( topLeft, bottomRight, Map::GridBorderColour );

	// Vertical grid lines
	for ( int i = 1; i < Width; i++ )
	{
		Vei2 top = (Vei2)Location + Vei2( (int)std::ceil(i * CellSize), 0 );
		Vei2 bottom( (int)std::ceil( Location.x ) + (int)std::ceil( i * CellSize ), (int)std::ceil( Location.y ) + (int)std::ceil( Height * CellSize ) - 1 );
		gfx.DrawLine( top, bottom, Map::GridColour );
	}

	// Horizontal grid lines
	for ( int j = 1; j < Height; j++ )
	{
		Vei2 left = (Vei2)Location + Vei2( 0, (int)std::ceil(j * CellSize) );
		Vei2 right( (int)std::ceil( Location.x ) + (int)std::ceil( Width * CellSize ) - 1, (int)std::ceil( Location.y ) + (int)std::ceil( j * CellSize ) );
		gfx.DrawLine( left, right, Map::GridColour );
	}
}

void Map::DoMouseEvents( Mouse & mouse )
{
	while ( !mouse.IsEmpty() )
	{
		Mouse::Event e = mouse.Read();
		Mouse::Event::Type meType = e.GetType();

		switch ( meType )
		{
		case Mouse::Event::Type::LPress:
			Click( e.GetPos() );
			break;
		case Mouse::Event::Type::LRelease:
			MouseInf.LMouseButtonGridLocation = Vei2( -1, -1 );
			break;
		case Mouse::Event::Type::Move:
			if ( e.LeftIsPressed() )
			{
				Click( e.GetPos() );
			}

			if ( e.MiddleIsPressed() )
			{
				const Vei2 temp = e.GetPos();
				const Vei2 delta = temp - MouseInf.MMouseButtonLocation;
				MouseInf.MMouseButtonLocation = temp;
				Move( (Vec2)delta );
			}

			if ( e.RightIsPressed() )
			{
				Clear( e.GetPos() );
			}
			break;
		case Mouse::Event::Type::MPress:
			MouseInf.MMouseButtonLocation = e.GetPos();
			break;
		case Mouse::Event::Type::RPress:
			Clear( e.GetPos() );
			break;
		case Mouse::Event::Type::WheelUp:
		case Mouse::Event::Type::WheelDown:
		{
			float zoomLevel = ZoomLevel * (meType == Mouse::Event::Type::WheelUp ? Map::ZoomFactor : Map::ZoomFactorInverse);
			zoomLevel = std::max( std::min( zoomLevel, Map::MaximumZoomLevel ), Map::MinimumZoomLevel );
			Zoom( (Vec2)e.GetPos(), zoomLevel );
		}
		break;
		default:
			break;
		}
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
	CellSize = Map::DefaultCellSize * ZoomLevel;
	Location += deltaLocation;
}

void Map::Clear( const Vei2& screenLocation )
{
	const Vei2 gridLocation = ScreenToGrid( screenLocation );
	if ( !IsOnGrid( gridLocation ) )
	{
		return;
	}

	Cells.at( gridLocation ).Clear();
}

void Map::Click( const Vei2& screenLocation )
{
	const Vei2 gridLocation = ScreenToGrid( screenLocation );
	if ( !IsOnGrid( gridLocation ) )
	{
		return;
	}

	if ( gridLocation == MouseInf.LMouseButtonGridLocation )
	{
		// Don't want to be automatically clicking the same thing more than once
		return;
	}

	MouseInf.LMouseButtonGridLocation = gridLocation;
	Cells.at( gridLocation ).Click();
}

bool Map::IsOnGrid( const Vei2& gridLocation )
{
	return gridLocation.x >= 0 && gridLocation.y >= 0 &&
		gridLocation.x < Width && gridLocation.y < Height;
}

const Vei2 Map::ScreenToGrid( const Vei2& screenLocation )
{
	const Vec2 gridLocationF = (Vec2)screenLocation - (Vec2)Location;
	const Vei2 gridLocation( (int)(gridLocationF.x / CellSize), (int)(gridLocationF.y / CellSize) );
	return gridLocation;
}
