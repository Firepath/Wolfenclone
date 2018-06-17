#include "Grid.h"
#include "SpriteEffect.h"

Grid::Cell::Cell( const Vei2& location )
	:
	Location( location )
{
}

void Grid::Cell::Clear()
{
	Surf = nullptr;
	Fill( Colors::Black );
}

void Grid::Cell::Draw( const Grid& map, Graphics& gfx ) const
{
	if ( NothingToDraw() )
	{
		return;
	}

	const Vei2 mapScreenLocation = map.GetScreenLocation();
	Vei2 topLeft = mapScreenLocation + Vei2( (int)std::ceil( (float)Location.x * map.CellSize ), (int)std::ceil( (float)Location.y * map.CellSize ) );
	Vei2 bottomRight = mapScreenLocation + Vei2( (int)std::ceil( (float)(Location.x + 1) * map.CellSize ) - 1, (int)std::ceil( (float)(Location.y + 1) * map.CellSize ) - 1 );

	if ( Surf != nullptr )
	{
		gfx.DrawSprite( RectI( topLeft, bottomRight ), *Surf, SpriteEffect::Copy() );
	}
	else
	{
		Color colour = Colour;
		if ( IsEmpty() && IsEnclosed() )
		{
			colour = EditConstants::CellConstants::CellEnclosedColour;
		}

		gfx.DrawBox( topLeft, bottomRight, colour, PixelEffect::Copy() );
	}
}

const bool Grid::Cell::Fill( const Color colour )
{
	bool empty = IsEmpty();
	Colour = colour;

	if ( IsEnclosed() && !IsEmpty() )
	{
		SetEnclosed( false );
	}

	return empty;
}

const bool Grid::Cell::Fill( Surface* const surface )
{
	bool empty = IsEmpty();
	Surf = surface;

	if ( IsEnclosed() && !IsEmpty() )
	{
		SetEnclosed( false );
	}

	return empty;
}

const Vei2& Grid::Cell::GetLocation() const
{
	return Location;
}

const bool Grid::Cell::IsEmpty() const
{
	return Colour == Colors::Black && Surf == nullptr;
}

const bool Grid::Cell::IsEnclosed() const
{
	return Enclosed;
}

void Grid::Cell::SetEnclosed( bool enclosed )
{
	Enclosed = enclosed;
}

const bool Grid::Cell::NothingToDraw() const
{
	return IsEmpty() && !IsEnclosed();
}
