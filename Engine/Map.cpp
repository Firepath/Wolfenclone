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
	const Vei2 screenLocation = ScreenLocation();

	for ( int j = 0; j < Height; j++ )
	{
		const int y = screenLocation.y + (int)std::ceil( j * CellSize );
		const int y1 = screenLocation.y + (int)std::ceil( (j + 1) * CellSize ) - 1;
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
			const int x = screenLocation.x + (int)std::ceil( i * CellSize );
			const int x1 = screenLocation.x + (int)std::ceil( (i + 1) * CellSize ) - 1;
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

	auto cellIt = Cells.find( MouseInf.HoverGridLocation );
	if ( cellIt != Cells.end() )
	{
		cellIt->second.Hover( *this, gfx );
	}

	// Grid border
	Vei2 topLeft = screenLocation;
	Vei2 bottomRight = topLeft + Vei2( (int)std::ceil( Width * CellSize ) - 1, (int)std::ceil( Height * CellSize ) - 1 );
	gfx.DrawBoxBorder( topLeft, bottomRight, Map::GridBorderColour );

	// Vertical grid lines
	for ( int i = 1; i < Width; i++ )
	{
		Vei2 top = screenLocation + Vei2( (int)std::ceil(i * CellSize), 0 );
		Vei2 bottom = top + Vei2( 0, (int)std::ceil( Height * CellSize ) - 1 );
		gfx.DrawLine( top, bottom, Map::GridColour );
	}

	// Horizontal grid lines
	for ( int j = 1; j < Height; j++ )
	{
		Vei2 left = screenLocation + Vei2( 0, (int)std::ceil(j * CellSize) );
		Vei2 right = left + Vei2( (int)std::ceil( Width * CellSize ) - 1, 0 );
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
			{
				const Vei2 mousePos = e.GetPos();

				MouseInf.HoverGridLocation = ScreenToGrid( mousePos );

				if ( e.LeftIsPressed() )
				{
					Click( mousePos );
				}

				if ( e.MiddleIsPressed() )
				{
					const Vei2 temp = mousePos;
					const Vei2 delta = temp - MouseInf.MMouseButtonLocation;
					MouseInf.MMouseButtonLocation = temp;
					Move( (Vec2)delta );
				}

				if ( e.RightIsPressed() )
				{
					Clear( mousePos );
				}
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
			Zoom( (Vec2)e.GetPos(), meType == Mouse::Event::Type::WheelUp );
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

void Map::Zoom( const Vec2& zoomLocation, const bool zoomingIn )
{
	float zoomLevel = ZoomLevel * (zoomingIn ? Map::ZoomFactor : Map::ZoomFactorInverse);
	zoomLevel = std::max( std::min( zoomLevel, Map::MaximumZoomLevel ), Map::MinimumZoomLevel );
	const float newCellSize = std::max( Map::DefaultCellSize * zoomLevel, Map::MinimumCellSize );

	if ( newCellSize == CellSize )
	{
		// No zooming, nothing to do
		return;
	}

	const float ratio = newCellSize / CellSize;
	zoomLevel = ratio * ZoomLevel;

	const Vec2 oldDistance = Location - zoomLocation;
	const Vec2 newDistance = oldDistance * ratio;
	const Vec2 deltaLocation = newDistance - oldDistance;

	ZoomLevel = zoomLevel;
	CellSize = newCellSize;
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

const Vei2 Map::ScreenLocation() const
{
	return Vei2( std::ceil( Location.x ), std::ceil( Location.y ) );
}

const Vei2 Map::ScreenToGrid( const Vei2& screenLocation )
{
	const Vec2 gridLocationF = (Vec2)screenLocation - (Vec2)Location;
	if ( gridLocationF.x < 0.0f || gridLocationF.y < 0.0f )
	{
		return Vei2( -1, -1 );
	}

	const Vei2 gridLocation( (int)(gridLocationF.x / CellSize), (int)(gridLocationF.y / CellSize) );
	return gridLocation;
}
