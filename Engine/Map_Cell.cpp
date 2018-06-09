#include "Map.h"

Map::Cell::Cell( const Vei2& location )
	:
	Location( location )
{
}

void Map::Cell::Clear()
{
	Fill( Colors::Black );
}

void Map::Cell::Draw( const Map& map, Graphics& gfx ) const
{
	if ( NothingToDraw() )
	{
		return;
	}

	Color colour = Colour;
	if ( IsEmpty() && IsEnclosed() )
	{
		colour = Cell::CellEnclosedColour;
	}

	const Vei2 mapScreenLocation = map.ScreenLocation();
	Vei2 topLeft = mapScreenLocation + Vei2( (int)std::ceil( (float)Location.x * map.CellSize ), (int)std::ceil( (float)Location.y * map.CellSize ) );
	Vei2 bottomRight = mapScreenLocation + Vei2( (int)std::ceil( (float)(Location.x + 1) * map.CellSize ) - 1, (int)std::ceil( (float)(Location.y + 1) * map.CellSize ) - 1 );

	gfx.DrawBox( topLeft, bottomRight, colour, PixelEffect::Copy() );
}

const bool Map::Cell::Fill( const Color colour )
{
	bool empty = IsEmpty();
	Colour = colour;

	if ( IsEnclosed() && !IsEmpty() )
	{
		SetEnclosed( false );
	}

	return empty;
}

const Vei2& Map::Cell::GetLocation() const
{
	return Location;
}

const bool Map::Cell::IsEmpty() const
{
	return Colour == Colors::Black;
}

const bool Map::Cell::IsEnclosed() const
{
	return Enclosed;
}

void Map::Cell::SetEnclosed( bool enclosed )
{
	Enclosed = enclosed;
}

const bool Map::Cell::NothingToDraw() const
{
	return IsEmpty() && !IsEnclosed();
}
