#include <algorithm>

#include "Map.h"
#include "VectorExtensions.h"

Map::Map( const int width, const int height, const Vec2& location )
	:
	CellSize( Map::DefaultCellSize ),
	Width( width ),
	Height( height ),
	Location( location )
{
	Cells = std::make_unique<std::unordered_map<Vei2, Cell, Vei2::Hasher>>();
	for ( int j = 0; j < Height; j++ )
	{
		for ( int i = 0; i < Width; i++ )
		{
			Vei2 l( i, j );
			Cell c( l );
			Cells->emplace( l, c );
		}
	}
}

void Map::ClearCell( const Vei2& gridLocation )
{
	if ( !IsOnGrid( gridLocation ) )
	{
		return;
	}

	Cell& cell = GetCell( gridLocation );
	cell.Clear();

	if ( !FillClosedArea( gridLocation ) )
	{
		// Check for, and clear enclosedness on surrounding cells
		ClearEnclosedCells( gridLocation );
	}
}

void Map::Draw( Graphics& gfx )
{
	const Vei2 screenLocation = ScreenLocation();

	DrawCells( screenLocation, gfx );
	DrawGrid( screenLocation, gfx );
}

void Map::Fill( const Vei2 & gridLocation, const Color colour )
{
	Cell& cell = GetCell( gridLocation );
	const bool wasEnclosed = cell.IsEnclosed();
	if ( cell.Fill( colour ) )
	{
		CheckForClosingArea( gridLocation, wasEnclosed );
	}
}

void Map::Fill( const Vei2 & gridLocation, Surface * const surface )
{
	Cell& cell = GetCell( gridLocation );
	const bool wasEnclosed = cell.IsEnclosed();
	if ( cell.Fill( surface ) )
	{
		CheckForClosingArea( gridLocation, wasEnclosed );
	}
}

Map::Cell& Map::GetCell( const Vei2 & gridLocation ) const
{
	assert( IsOnGrid( gridLocation ) );
	return Cells->at( gridLocation );
}

void Map::HighlightCell( const Vei2& gridLocation, const Color highlightColour, const float highlightOpacity, Graphics & gfx ) const
{
	const Vei2 mapScreenLocation = ScreenLocation();
	Vei2 topLeft = mapScreenLocation + Vei2( (int)std::ceil( (float)gridLocation.x * CellSize ), (int)std::ceil( (float)gridLocation.y * CellSize ) );
	Vei2 bottomRight = mapScreenLocation + Vei2( (int)std::ceil( (float)(gridLocation.x + 1) * CellSize ) - 1, (int)std::ceil( (float)(gridLocation.y + 1) * CellSize ) - 1 );

	PixelEffect::Transparency effect( Colors::Magenta, highlightOpacity );
	gfx.DrawBox( topLeft, bottomRight, highlightColour, effect );
}

bool Map::IsOnGrid( const Vei2& gridLocation ) const
{
	return gridLocation.x >= 0 && gridLocation.y >= 0 &&
		gridLocation.x < Width && gridLocation.y < Height;
}

void Map::Move( const Vec2 & delta )
{
	Location += delta;
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

void Map::ClearEnclosedCells( const Vei2 & gridLocation )
{
	std::unique_ptr<std::vector<Vei2>> checked = std::make_unique<std::vector<Vei2>>(); checked->reserve( Width * Height );
	std::unique_ptr<std::vector<Vei2>> toBeChecked = std::make_unique<std::vector<Vei2>>(); toBeChecked->reserve( Width * Height );
	toBeChecked->emplace_back( gridLocation );

	auto NeedsToBeChecked = [this]( const Vei2& location, std::unique_ptr<std::vector<Vei2>>& checked, std::unique_ptr<std::vector<Vei2>>& toBeChecked )
	{
		return 
			this->IsOnGrid( location ) &&
			this->GetCell( location ).IsEnclosed() &&
			!VectorExtension::Contains( checked, location ) && // Not already checked
			!VectorExtension::Contains( toBeChecked, location ); // Not already in the list to be checked
	};

	while ( toBeChecked->size() > 0 )
	{
		const Vei2 location = toBeChecked->back();
		toBeChecked->pop_back();
		GetCell( location ).SetEnclosed( false );
		checked->emplace_back( location );

		Vei2 left = location + Vei2( -1, 0 );
		if ( NeedsToBeChecked( left, checked, toBeChecked ) )
		{
			toBeChecked->emplace_back( left );
		}

		Vei2 right = location + Vei2( 1, 0 );
		if ( NeedsToBeChecked( right, checked, toBeChecked ) )
		{
			toBeChecked->emplace_back( right );
		}

		Vei2 top = location + Vei2( 0, -1 );
		if ( NeedsToBeChecked( top, checked, toBeChecked ) )
		{
			toBeChecked->emplace_back( top );
		}

		Vei2 bottom = location + Vei2( 0, 1 );
		if ( NeedsToBeChecked( bottom, checked, toBeChecked ) )
		{
			toBeChecked->emplace_back( bottom );
		}
	}
}

void Map::DrawCells( const Vei2 screenLocation, Graphics & gfx ) const
{
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
			GetCell( cellLocation ).Draw( *this, gfx );
		}
	}
}

void Map::DrawGrid( const Vei2 screenLocation, Graphics & gfx ) const
{
	// Grid border
	Vei2 topLeft = screenLocation;
	Vei2 bottomRight = topLeft + Vei2( (int)std::ceil( Width * CellSize ) - 1, (int)std::ceil( Height * CellSize ) - 1 );
	gfx.DrawBoxBorder( topLeft, bottomRight, Map::GridBorderColour );

	// Vertical grid lines
	for ( int i = 1; i < Width; i++ )
	{
		Vei2 top = screenLocation + Vei2( (int)std::ceil( i * CellSize ), 0 );
		Vei2 bottom = top + Vei2( 0, (int)std::ceil( Height * CellSize ) - 1 );
		gfx.DrawLine( top, bottom, Map::GridColour );
	}

	// Horizontal grid lines
	for ( int j = 1; j < Height; j++ )
	{
		Vei2 left = screenLocation + Vei2( 0, (int)std::ceil( j * CellSize ) );
		Vei2 right = left + Vei2( (int)std::ceil( Width * CellSize ) - 1, 0 );
		gfx.DrawLine( left, right, Map::GridColour );
	}
}

void Map::CheckForClosingArea( const Vei2 & gridLocation, const bool wasEnclosed )
{
	if ( IsJointFormed( gridLocation ) && !wasEnclosed )
	{
		// Fill the various potential enclosed directions
		FillClosedArea( gridLocation + Vei2( -1, 0 ) );
		FillClosedArea( gridLocation + Vei2( 1, 0 ) );
		FillClosedArea( gridLocation + Vei2( 0, -1 ) );
		FillClosedArea( gridLocation + Vei2( 0, 1 ) );
	}
}

const bool Map::FillClosedArea( const Vei2& gridLocation )
{
	auto cellIt = Cells->find( gridLocation );
	if ( cellIt == Cells->end() || !cellIt->second.IsEmpty() )
	{
		// Not on the grid, nothing to do
		// Or it is a filled cell, not an empty one being enclosed
		return false;
	}

	std::unique_ptr<std::vector<Vei2>> enclosed = std::make_unique<std::vector<Vei2>>(); enclosed->reserve( Width * Height );
	std::unique_ptr<std::vector<Vei2>> checked = std::make_unique<std::vector<Vei2>>(); checked->reserve(Width * Height);
	std::unique_ptr<std::vector<Vei2>> toBeChecked = std::make_unique<std::vector<Vei2>>(); toBeChecked->reserve(Width * Height);
	toBeChecked->emplace_back( cellIt->first );

	auto NeedsToBeChecked = [this]( const Vei2& location, std::unique_ptr<std::vector<Vei2>>& checked, std::unique_ptr<std::vector<Vei2>>& toBeChecked )
	{
		return
			GetCell( location ).IsEmpty() && // Already a wall if not empty
			!VectorExtension::Contains( checked, location ) && // Not already checked
			!VectorExtension::Contains( toBeChecked, location ); // Not already in the list to be checked
	};

	bool closed = true;
	while ( toBeChecked->size() > 0 )
	{
		const Vei2 location = toBeChecked->back();
		toBeChecked->pop_back();
		if ( IsCellEnclosed( location ) ) // This MUST mean it is not on the edge of the grid at least
		{
			enclosed->emplace_back( location );
			checked->emplace_back( location );

			Vei2 left = location + Vei2( -1, 0 );
			if ( NeedsToBeChecked( left, checked, toBeChecked ) )
			{
				toBeChecked->emplace_back( left );
			}

			Vei2 right = location + Vei2( 1, 0 );
			if ( NeedsToBeChecked( right, checked, toBeChecked ) )
			{
				toBeChecked->emplace_back( right );
			}

			Vei2 top = location + Vei2( 0, -1 );
			if ( NeedsToBeChecked( top, checked, toBeChecked ) )
			{
				toBeChecked->emplace_back( top );
			}

			Vei2 bottom = location + Vei2( 0, 1 );
			if ( NeedsToBeChecked( bottom, checked, toBeChecked ) )
			{
				toBeChecked->emplace_back( bottom );
			}
		}
		else
		{
			closed = false;
			break;
		}
	}

	if ( closed )
	{
		for ( Vei2 cell : *(enclosed.get()) )
		{
			GetCell( cell ).SetEnclosed( true );
		}
	}

	return closed;
}

const bool Map::FindWall( const Vei2 & gridLocation, const int xDirection, const int yDirection ) const
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
			if ( !GetCell( cell ).IsEmpty() )
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

const bool Map::IsCellEnclosed( const Vei2 & gridLocation ) const
{
	bool enclosed = FindWall( gridLocation, -1, 0 );
	if ( enclosed )
	{
		enclosed &= FindWall( gridLocation, 1, 0 );
		if ( enclosed )
		{
			enclosed &= FindWall( gridLocation, 0, -1 );
			if ( enclosed )
			{
				enclosed &= FindWall( gridLocation, 0, 1 );
			}
		}
	}

	return enclosed;
}

bool Map::IsJointFormed( const Vei2& gridLocation ) const
{
	const int startX = std::max( 0, std::min( Width - 1, gridLocation.x - 1 ) );
	const int startY = std::max( 0, std::min( Height - 1, gridLocation.y - 1 ) );
	const int endX = std::max( 0, std::min( Width - 1, gridLocation.x + 1 ) );
	const int endY = std::max( 0, std::min( Height - 1, gridLocation.y + 1 ) );

	bool found = false;

	// If there is a filled cell to the left, check if there is one on the right,
	if ( startX < gridLocation.x && endX > gridLocation.x )
	{
		bool left = false;
		for ( int i = startY; i <= endY; i++ )
		{
			if ( !GetCell( Vei2( startX, i ) ).IsEmpty() )
			{
				left = true;
				break;
			}
		}

		if ( left )
		{
			for ( int i = startY; i <= endY; i++ )
			{
				if ( !GetCell( Vei2( endX, i ) ).IsEmpty() )
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
			if ( !GetCell( Vei2( i, startY ) ).IsEmpty() )
			{
				top = true;
				break;
			}
		}

		if ( top )
		{
			for ( int i = startX; i <= endX; i++ )
			{
				if ( !GetCell( Vei2( i, endY ) ).IsEmpty() )
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

const Vei2 Map::ScreenLocation() const
{
	return Vei2( (int)std::ceil( Location.x ), (int)std::ceil( Location.y ) );
}