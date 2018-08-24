#include <algorithm>

#include "Graphics.h"
#include "Grid.h"
#include "PixelEffect.h"
#include "Surface.h"
#include "VectorExtensions.h"

Grid::Grid( const int width, const int height, const Vec2& location )
	:
	CellSize( Grid::DefaultCellSize ),
	Width( width ),
	Height( height ),
	Location( location )
{
	Cells = std::make_unique<std::unordered_map<Vei2, Cell, Vei2::Hasher>>();
	EnclosedCells = std::make_unique<std::unordered_map<Vei2, bool, Vei2::Hasher>>();
}

void Grid::ClearSelectedCells()
{
	SelectedCells.clear();
}

void Grid::DeleteCell( const Vei2& gridLocation )
{
	if ( !IsOnGrid( gridLocation ) || !IsCellOccupied( gridLocation ) )
	{
		return;
	}

	EraseCell( gridLocation );

	if ( IsCellAlreadyEnclosed( gridLocation ) )
	{
		EnclosedCells->emplace( gridLocation, true );
	}
	else
	{
		// Check for, and clear enclosedness on surrounding cells
		ClearEnclosedCells( gridLocation );
	}

	auto newEnd = std::remove_if( SelectedCells.begin(), SelectedCells.end(), [this]( Vei2 gridLocation ) { return !IsCellOccupied( gridLocation ); } );
	SelectedCells.erase( newEnd, SelectedCells.end() );
}

void Grid::DeleteSelectedCells()
{
	while ( SelectedCells.size() > 0 )
	{
		DeleteCell( SelectedCells.back() );
	}
}

void Grid::Draw( Graphics& gfx )
{
	const Vei2 screenLocation = GetScreenLocation();

	if ( !DrawGridOverCells )
	{
		DrawGrid( screenLocation, gfx );
	}

	DrawEnclosedCells( screenLocation, gfx );
	DrawCells( screenLocation, gfx );

	if ( DrawGridOverCells )
	{
		DrawGrid( screenLocation, gfx );
	}

	HighlightSelectedCells( gfx );
}

void Grid::Fill( const Vei2& gridLocation, const MapFixture* const fixture )
{
	assert( IsOnGrid( gridLocation ) );

	const bool wasEnclosed = IsCellEnclosed( gridLocation );
	bool wasEmpty = true;
	if ( IsCellOccupied( gridLocation ) )
	{
		EraseCell( gridLocation );
		wasEmpty = false;
	}

	Cells->emplace( gridLocation, Cell( gridLocation, fixture ) );

	Cell& cell = GetCell( gridLocation );
	if ( wasEmpty )
	{
		CheckForClosingArea( gridLocation, wasEnclosed );
	}

	if ( wasEnclosed )
	{
		EraseEnclosedCell( gridLocation );
	}
}

Grid::Cell& Grid::GetCell( const Vei2& gridLocation ) const
{
	assert( IsOnGrid( gridLocation ) );
	assert( IsCellOccupied( gridLocation ) );
	return Cells->at( gridLocation );
}

void Grid::HighlightCell( const Vei2& gridLocation, const Color highlightColour, const float highlightOpacity, const bool drawBorder, Graphics & gfx ) const
{
	const Vei2 mapScreenLocation = GetScreenLocation();
	const Vei2 topLeft = mapScreenLocation + Vei2( (int)std::ceil( (float)gridLocation.x * CellSize ), (int)std::ceil( (float)gridLocation.y * CellSize ) );
	const Vei2 bottomRight = mapScreenLocation + Vei2( (int)std::ceil( (float)(gridLocation.x + 1) * CellSize ) - 1, (int)std::ceil( (float)(gridLocation.y + 1) * CellSize ) - 1 );
	const RectI rect( topLeft, bottomRight );

	std::unique_ptr<PixelEffect::Effect> effect = std::make_unique<PixelEffect::Transparency>( highlightOpacity );
	gfx.DrawBox( rect, highlightColour, effect );

	if ( drawBorder )
	{
		gfx.DrawBoxBorder( rect, highlightColour, effect, GetCellBorderThickness() );
	}
}

bool Grid::IsOnGrid( const Vei2& gridLocation ) const
{
	return gridLocation.x >= 0 && gridLocation.y >= 0 &&
		gridLocation.x < Width && gridLocation.y < Height;
}

void Grid::Move( const Vec2 & delta )
{
	Location += delta;
}

const Vei2 Grid::ScreenToGrid( const Vei2& screenLocation ) const
{
	const Vec2 gridLocationF = (Vec2)screenLocation - (Vec2)Location;
	if ( gridLocationF.x < 0.0f || gridLocationF.y < 0.0f )
	{
		return Vei2( -1, -1 );
	}

	const Vei2 gridLocation( (int)(gridLocationF.x / CellSize), (int)(gridLocationF.y / CellSize) );
	return gridLocation;
}

void Grid::SetTemporarySelectedToSelected()
{
	// Move temporary selection into "permanent" selection
	for ( Vei2 location : TemporarySelectedCells )
	{
		if ( IsCellOccupied( location ) && !VectorExtension::Contains( SelectedCells, location ) )
		{
			SelectedCells.push_back( location );
		}
	}

	// remove from the selection list any cells that are empty
	auto newEnd = std::remove_if( SelectedCells.begin(), SelectedCells.end(), [this]( Vei2 gridLocation ) { return !IsCellOccupied( gridLocation ); } );
	SelectedCells.erase( newEnd, SelectedCells.end() );

	TemporarySelectedCells.clear();
}

void Grid::TemporarySelectCell( const Vei2 & gridLocation )
{
	if ( !VectorExtension::Contains( TemporarySelectedCells, gridLocation ) )
	{
		TemporarySelectedCells.push_back( gridLocation );
	}
}

void Grid::TemporarySelectCellsInRectangle( const RectI& rect )
{
	TemporarySelectedCells.clear();

	TemporarySelectedCells.reserve( rect.GetWidth() * rect.GetHeight() );

	for ( int x = rect.left; x <= rect.right; x++ )
	{
		for ( int y = rect.top; y <= rect.bottom; y++ )
		{
			TemporarySelectedCells.push_back( Vei2( x, y ) );
		}
	}
}

void Grid::ToggleGridDrawing()
{
	DrawGridOverCells = !DrawGridOverCells;
}

void Grid::Zoom( const Vec2& zoomLocation, const bool zoomingIn )
{
	float zoomLevel = ZoomLevel * (zoomingIn ? Grid::ZoomFactor : Grid::ZoomFactorInverse);
	zoomLevel = std::max( std::min( zoomLevel, Grid::MaximumZoomLevel ), Grid::MinimumZoomLevel );
	const float newCellSize = std::max( Grid::DefaultCellSize * zoomLevel, Grid::MinimumCellSize );

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

const bool Grid::CheckIfCellIsEnclosed( const Vei2& gridLocation ) const
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

void Grid::ClearEnclosedCells( const Vei2& gridLocation )
{
	std::unique_ptr<std::vector<Vei2>> checked = std::make_unique<std::vector<Vei2>>(); checked->reserve( Width * Height );
	std::unique_ptr<std::vector<Vei2>> toBeChecked = std::make_unique<std::vector<Vei2>>(); toBeChecked->reserve( Width * Height );
	toBeChecked->emplace_back( gridLocation );

	auto NeedsToBeChecked = [this]( const Vei2& location, std::unique_ptr<std::vector<Vei2>>& checked, std::unique_ptr<std::vector<Vei2>>& toBeChecked )
	{
		return 
			this->IsOnGrid( location ) &&
			this->IsCellEnclosed( location ) &&
			!VectorExtension::Contains( checked, location ) && // Not already checked
			!VectorExtension::Contains( toBeChecked, location ); // Not already in the list to be checked
	};

	while ( toBeChecked->size() > 0 )
	{
		const Vei2 location = toBeChecked->back();
		toBeChecked->pop_back();
		EraseEnclosedCell( location );
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

void Grid::DrawCells( const Vei2& screenLocation, Graphics & gfx ) const
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
			if ( IsCellOccupied( cellLocation ) )
			{
				GetCell( cellLocation ).Draw( *this, gfx );
			}
		}
	}
}

void Grid::DrawEnclosedCells( const Vei2& screenLocation, Graphics & gfx ) const
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
			if ( IsCellEnclosed( cellLocation ) )
			{
				DrawEnclosedCell( screenLocation, cellLocation, gfx );
			}
		}
	}
}

void Grid::DrawEnclosedCell( const Vei2 & screenLocation, const Vei2 & gridLocation, Graphics & gfx ) const
{
	Vei2 topLeft = screenLocation + Vei2( (int)std::ceil( (float)gridLocation.x * CellSize ), (int)std::ceil( (float)gridLocation.y * CellSize ) );
	Vei2 bottomRight = screenLocation + Vei2( (int)std::ceil( (float)(gridLocation.x + 1) * CellSize ) - 1, (int)std::ceil( (float)(gridLocation.y + 1) * CellSize ) - 1 );
	std::unique_ptr<PixelEffect::Effect> copy = std::make_unique<PixelEffect::Copy>();
	gfx.DrawBox( topLeft, bottomRight, EditConstants::CellConstants::CellEnclosedColour, copy );
}

void Grid::DrawGrid( const Vei2 screenLocation, Graphics & gfx ) const
{
	std::unique_ptr<PixelEffect::Effect> copy = std::make_unique<PixelEffect::Copy>();

	// Grid border
	Vei2 topLeft = screenLocation;
	Vei2 bottomRight = topLeft + Vei2( (int)std::ceil( Width * CellSize ) - 1, (int)std::ceil( Height * CellSize ) - 1 );
	gfx.DrawBoxBorder( RectI( topLeft, bottomRight ), Grid::GridBorderColour, copy, GetCellBorderThickness() );

	// Vertical grid lines
	for ( int i = 1; i < Width; i++ )
	{
		Vei2 top = screenLocation + Vei2( (int)std::ceil( i * CellSize ), 0 );
		Vei2 bottom = top + Vei2( 0, (int)std::ceil( Height * CellSize ) - 1 );
		gfx.DrawLine( top, bottom, Grid::GridColour, copy );
	}

	// Horizontal grid lines
	for ( int j = 1; j < Height; j++ )
	{
		Vei2 left = screenLocation + Vei2( 0, (int)std::ceil( j * CellSize ) );
		Vei2 right = left + Vei2( (int)std::ceil( Width * CellSize ) - 1, 0 );
		gfx.DrawLine( left, right, Grid::GridColour, copy );
	}
}

void Grid::EraseCell( const Vei2 & gridLocation )
{
	assert( IsOnGrid( gridLocation ) );

	if ( IsCellOccupied( gridLocation ) )
	{
		Cells->erase( gridLocation );
	}
}

void Grid::EraseEnclosedCell( const Vei2 & gridLocation )
{
	assert( IsOnGrid( gridLocation ) );

	if ( IsCellEnclosed( gridLocation ) )
	{
		EnclosedCells->erase( gridLocation );
	}
}
void Grid::CheckForClosingArea( const Vei2 & gridLocation, const bool wasEnclosed )
{
	if ( !wasEnclosed && IsJointFormed( gridLocation ) )
	{
		// Fill the various potential enclosed directions
		FillClosedArea( gridLocation + Vei2( -1, 0 ) );
		FillClosedArea( gridLocation + Vei2( 1, 0 ) );
		FillClosedArea( gridLocation + Vei2( 0, -1 ) );
		FillClosedArea( gridLocation + Vei2( 0, 1 ) );
	}
}

const bool Grid::FillClosedArea( const Vei2& gridLocation )
{
	if ( !IsOnGrid( gridLocation ) || IsCellOccupied( gridLocation ) )
	{
		// Not on the grid, nothing to do
		// Or it is a filled cell, not an empty one being enclosed
		return false;
	}

	std::unique_ptr<std::vector<Vei2>> enclosed = std::make_unique<std::vector<Vei2>>(); enclosed->reserve( Width * Height );
	std::unique_ptr<std::vector<Vei2>> checked = std::make_unique<std::vector<Vei2>>(); checked->reserve(Width * Height);
	std::unique_ptr<std::vector<Vei2>> toBeChecked = std::make_unique<std::vector<Vei2>>(); toBeChecked->reserve(Width * Height);
	toBeChecked->emplace_back( gridLocation );

	auto NeedsToBeChecked = [this]( const Vei2& location, std::unique_ptr<std::vector<Vei2>>& checked, std::unique_ptr<std::vector<Vei2>>& toBeChecked )
	{
		return
			!IsCellOccupied( location ) && // Already a wall if not empty
			!VectorExtension::Contains( checked, location ) && // Not already checked
			!VectorExtension::Contains( toBeChecked, location ); // Not already in the list to be checked
	};

	bool closed = true;
	while ( toBeChecked->size() > 0 )
	{
		const Vei2 location = toBeChecked->back();
		toBeChecked->pop_back();
		if ( CheckIfCellIsEnclosed( location ) ) // This MUST mean it is not on the edge of the grid at least
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
		for ( Vei2 location : *(enclosed.get()) )
		{
			EnclosedCells->emplace( location, true );
		}
	}

	return closed;
}

const bool Grid::FindWall( const Vei2 & gridLocation, const int xDirection, const int yDirection ) const
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
			const Vei2 location( x, y );
			if ( IsCellOccupied( location ) )
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

const int Grid::GetCellBorderThickness() const
{
	return std::max( 2, (int)(ZoomLevel / 2.0f) );
}

const Vei2 Grid::GetScreenLocation() const
{
	return Vei2( (int)std::ceil( Location.x ), (int)std::ceil( Location.y ) );
}

const Vei2 Grid::GetSize() const
{
	return Vei2( Width, Height );
}

void Grid::HighlightSelectedCells( Graphics& gfx ) const
{
	for ( Vei2 gridLocation : SelectedCells )
	{
		HighlightCell( gridLocation, EditConstants::CellSelection::SelectModeHoverColour, EditConstants::CellSelection::CellHoverOpacity, true, gfx );
	}

	Vei2 topLeft = GetSize();
	Vei2 bottomRight( -1, -1 );
	for ( Vei2 gridLocation : TemporarySelectedCells )
	{
		if ( !VectorExtension::Contains( SelectedCells, gridLocation ) ) // Already drawn (above)
		{
			HighlightCell( gridLocation, EditConstants::CellSelection::SelectModeHoverColour, EditConstants::CellSelection::CellHoverOpacity, false, gfx );
		}

		if ( gridLocation.x < topLeft.x ) { topLeft.x = gridLocation.x; }
		if ( gridLocation.y < topLeft.y ) { topLeft.y = gridLocation.y; }
		if ( gridLocation.x > bottomRight.x ) { bottomRight.x = gridLocation.x; }
		if ( gridLocation.y > bottomRight.y ) { bottomRight.y = gridLocation.y; }
	}

	if ( IsOnGrid( topLeft ) && IsOnGrid( bottomRight ) )
	{
		const Vei2 gridLocation = GetScreenLocation();
		topLeft = (Vei2)((Vec2)topLeft * CellSize) + gridLocation;
		bottomRight = (Vei2)((Vec2)(bottomRight + Vei2( 1, 1 )) * CellSize) + gridLocation;

		std::unique_ptr<PixelEffect::Effect> effect = std::make_unique<PixelEffect::Transparency>( EditConstants::CellSelection::CellHoverOpacity );
		gfx.DrawBoxBorder( RectI( topLeft, bottomRight ), EditConstants::CellSelection::SelectModeHoverColour, effect, GetCellBorderThickness() );
	}
}

const bool Grid::IsCellAlreadyEnclosed( const Vei2 & gridLocation ) const
{
	const Vei2 left( gridLocation + Vei2( -1, 0 ) );
	const Vei2 right( gridLocation + Vei2( 1, 0 ) );
	const Vei2 top( gridLocation + Vei2( 0, -1 ) );
	const Vei2 bottom( gridLocation + Vei2( 0, 1 ) );

	auto checkCell = [this]( const Vei2& location )
	{
		return
			IsOnGrid( location ) && ( IsCellOccupied( location ) || IsCellEnclosed( location ) );
	};

	return
		checkCell( left ) && checkCell( right ) && checkCell( top ) && checkCell( bottom );
}

const bool Grid::IsCellEnclosed( const Vei2 & gridLocation ) const
{
	assert( IsOnGrid( gridLocation ) );
	return EnclosedCells->find( gridLocation ) != EnclosedCells->end();
}

const bool Grid::IsCellOccupied( const Vei2 & gridLocation ) const
{
	assert( IsOnGrid( gridLocation ) );
	return Cells->find( gridLocation ) != Cells->end();
}

bool Grid::IsJointFormed( const Vei2& gridLocation ) const
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
			const Vei2 location( startX, i );
			if ( IsCellOccupied( location ) )
			{
				left = true;
				break;
			}
		}

		if ( left )
		{
			for ( int i = startY; i <= endY; i++ )
			{
				const Vei2 location( endX, i );
				if ( IsCellOccupied( location ) )
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
			const Vei2 location( i, startY );
			if ( IsCellOccupied( location ) )
			{
				top = true;
				break;
			}
		}

		if ( top )
		{
			for ( int i = startX; i <= endX; i++ )
			{
				const Vei2 location( i, endY );
				if ( IsCellOccupied( location ) )
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