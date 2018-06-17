#include "Grid.h"
#include "SpriteEffect.h"

Grid::Cell::Cell( const Vei2& location, Surface* const surface )
	:
	Location( location ),
	Surf( surface )
{
}

void Grid::Cell::Draw( const Grid& map, Graphics& gfx ) const
{
	const Vei2 mapScreenLocation = map.GetScreenLocation();
	Vei2 topLeft = mapScreenLocation + Vei2( (int)std::ceil( (float)Location.x * map.CellSize ), (int)std::ceil( (float)Location.y * map.CellSize ) );
	Vei2 bottomRight = mapScreenLocation + Vei2( (int)std::ceil( (float)(Location.x + 1) * map.CellSize ) - 1, (int)std::ceil( (float)(Location.y + 1) * map.CellSize ) - 1 );

	if ( Surf != nullptr )
	{
		gfx.DrawSprite( RectI( topLeft, bottomRight ), *Surf, SpriteEffect::Copy() );
	}
}

const Vei2& Grid::Cell::GetLocation() const
{
	return Location;
}