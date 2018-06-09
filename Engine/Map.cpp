#include <algorithm>

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

const bool Map::IsCellEnclosed( const Vei2 & gridLocation ) const
{
	const auto left = Cells.find( gridLocation + Vei2( -1, 0 ) );
	const auto right = Cells.find( gridLocation + Vei2( 1, 0 ) );
	const auto top = Cells.find( gridLocation + Vei2( 0, -1 ) );
	const auto bottom = Cells.find( gridLocation + Vei2( 0, 1 ) );

	// If all the surrounding cells are either walls or are enclosed, this cell is enclosed
	return
		left != Cells.end() && (left->second.IsEnclosed() || !left->second.Empty()) &&
		right != Cells.end() && (right->second.IsEnclosed() || !right->second.Empty()) &&
		top != Cells.end() && (top->second.IsEnclosed() || !top->second.Empty()) &&
		bottom != Cells.end() && (bottom->second.IsEnclosed() || !bottom->second.Empty());
}

void Map::Clear( const Vei2& screenLocation )
{
	const Vei2 gridLocation = ScreenToGrid( screenLocation );
	if ( !IsOnGrid( gridLocation ) )
	{
		return;
	}

	Cell& cell = Cells.at( gridLocation );
	cell.Clear();

	if ( IsCellEnclosed( gridLocation ) )
	{
		cell.SetEnclosed( true );
	}
	else
	{
		// Check for, and clear enclosedness on surrounding cells
	}
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
	if ( Cells.at( gridLocation ).Fill( Colors::White ) )
	{
		if ( IsJointFormed( gridLocation ) )
		{
			// Fill the various potential enclosed directions
			FindClosedArea( gridLocation + Vei2( -1, 0 ) );
			FindClosedArea( gridLocation + Vei2( 1, 0 ) );
			FindClosedArea( gridLocation + Vei2( 0, -1 ) );
			FindClosedArea( gridLocation + Vei2( 0, 1 ) );
		}
	}
}

void Map::FindClosedArea( const Vei2& gridLocation )
{
	const auto cellIt = Cells.find( gridLocation );
	if ( cellIt == Cells.end() || !cellIt->second.Empty() )
	{
		// Not on the grid, nothing to do
		// Or it is a filled cell, not an empty one being enclosed
		return;
	}

	std::vector<Vei2> checkedLocations;
	const bool closed = FindClosedArea( gridLocation, checkedLocations );

	if ( closed )
	{
		for ( Vei2 cell : checkedLocations )
		{
			Cells.at( cell ).SetEnclosed( true );
		}
	}
}

const bool Map::FindClosedArea( const Vei2& gridLocation, std::vector<Vei2>& checkedLocations )
{
	//const std::unordered_map<Vei2, Map::Cell, Vei2::Hasher>::iterator it = Cells.find( gridLocation );
	const auto it = Cells.find( gridLocation );
	if ( it == Cells.end() )
	{
		// We've gone off the grid, not enclosed.
		return false;
	}
	else if ( !it->second.Empty() )
	{
		// Checking a wall, we've reached an end point
		return true;
	}
	else if ( std::find( checkedLocations.begin(), checkedLocations.end(), gridLocation ) != checkedLocations.end() )
	{
		// Checking a location we've already checked
		return true;
	}

	// Check if this cell is enclosed in all directions
	bool enclosed = FindOpposingWall( gridLocation, -1, 0 );
	if ( enclosed )
	{
		enclosed &= FindOpposingWall( gridLocation, 1, 0 );
		if ( enclosed )
		{
			enclosed &= FindOpposingWall( gridLocation, 0, -1 );
			if ( enclosed )
			{
				enclosed &= FindOpposingWall( gridLocation, 0, 1 );
			}
		}
	}

	if ( !enclosed )
	{
		// Not enclosed
		return false;
	}

	// This cell is enclosed, continue checking

	checkedLocations.push_back( gridLocation );

	enclosed &= FindClosedArea( gridLocation - Vei2( -1, 0 ), checkedLocations );
	if ( enclosed )
	{
		enclosed &= FindClosedArea( gridLocation - Vei2( 1, 0 ), checkedLocations );
		if ( enclosed )
		{
			enclosed &= FindClosedArea( gridLocation - Vei2( 0, -1 ), checkedLocations );
			if ( enclosed )
			{
				enclosed &= FindClosedArea( gridLocation - Vei2( 0, 1 ), checkedLocations );
			}
		}
	}

	return enclosed;
}

const bool Map::FindOpposingWall( const Vei2 & gridLocation, const int xDirection, const int yDirection ) const
{
	// One of either direction must be zero and the other not zero
	assert( xDirection != 0 && yDirection == 0 || yDirection != 0 && xDirection == 0 );
	assert( std::abs( xDirection ) <= 1 );
	assert( std::abs( yDirection ) <= 1 );

	bool found = false;
	for ( int x = gridLocation.x + xDirection; x >= 0 && x < Width; x += xDirection )
	{
		for ( int y = gridLocation.y + yDirection; y >= 0 && y < Height; y += yDirection )
		{
			const Vei2 cell( x, y );
			if ( !Cells.at( cell ).Empty() )
			{
				// We found one that could create an enclosing space
				found = true;
				break;
			}

			if ( yDirection == 0 )
			{
				break;
			}
		}

		if ( xDirection == 0 )
		{
			break;
		}
	}

	return found;
}

bool Map::IsJointFormed( const Vei2& gridLocation ) const
{
	const int startX = std::max( 0, std::min( Width, gridLocation.x - 1 ) );
	const int startY = std::max( 0, std::min( Height, gridLocation.y - 1 ) );
	const int endX = std::max( 0, std::min( Width, gridLocation.x + 1 ) );
	const int endY = std::max( 0, std::min( Height, gridLocation.y + 1 ) );

	bool found = false;

	// If there is a filled cell to the left, check if there is one on the right,
	if ( startX < gridLocation.x && endX > gridLocation.x )
	{
		bool left = false;
		for ( int i = startY; i <= endY; i++ )
		{
			if ( !Cells.at( Vei2( startX, i ) ).Empty() )
			{
				left = true;
				break;
			}
		}

		if ( left )
		{
			for ( int i = startY; i <= endY; i++ )
			{
				if ( !Cells.at( Vei2( endX, i ) ).Empty() )
				{
					found = true;
					break;
				}
			}
		}
	}

	// Otherwise, if there is a filled cell on top, check if there is one on the bottom.
	if ( !found &&
		startY < gridLocation.y && endY > gridLocation.y )
	{
		bool top = false;
		for ( int i = startX; i <= endX; i++ )
		{
			if ( !Cells.at( Vei2( i, startY ) ).Empty() )
			{
				top = true;
				break;
			}
		}

		if ( top )
		{
			for ( int i = startX; i <= endX; i++ )
			{
				if ( !Cells.at( Vei2( i, endY ) ).Empty() )
				{
					found = true;
					break;
				}
			}
		}
	}

	// Otherwise, no NEW joint created by filling this grid location.

	return found;
}

bool Map::IsOnGrid( const Vei2& gridLocation ) const
{
	return gridLocation.x >= 0 && gridLocation.y >= 0 &&
		gridLocation.x < Width && gridLocation.y < Height;
}

const Vei2 Map::ScreenLocation() const
{
	return Vei2( (int)std::ceil( Location.x ), (int)std::ceil( Location.y ) );
}

const Vei2 Map::ScreenToGrid( const Vei2& screenLocation ) const
{
	const Vec2 gridLocationF = (Vec2)screenLocation - (Vec2)Location;
	if ( gridLocationF.x < 0.0f || gridLocationF.y < 0.0f )
	{
		return Vei2( -1, -1 );
	}

	const Vei2 gridLocation( (int)(gridLocationF.x / CellSize), (int)(gridLocationF.y / CellSize) );
	return gridLocation;
}
