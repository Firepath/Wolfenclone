#include "Graphics.h"
#include "Grid.h"
#include "MapFixture.h"
#include "PixelEffect.h"
#include "Surface.h"

Grid::Cell::Cell( const Vei2& location, const MapFixture* const fixture )
	:
	Location( location ),
	Fixture( fixture )
{
}

void Grid::Cell::Draw( const Grid& map, Graphics& gfx ) const
{
	const Vei2 mapScreenLocation = map.GetScreenLocation();
	Vei2 topLeft = mapScreenLocation + Vei2( (int)std::ceil( (float)Location.x * map.CellSize ), (int)std::ceil( (float)Location.y * map.CellSize ) );
	Vei2 bottomRight = mapScreenLocation + Vei2( (int)std::ceil( (float)(Location.x + 1) * map.CellSize ) - 1, (int)std::ceil( (float)(Location.y + 1) * map.CellSize ) - 1 );

	if ( Fixture != nullptr )
	{
		std::unique_ptr<PixelEffect::Effect> copy = std::make_unique<PixelEffect::Copy>();
		gfx.DrawSprite( RectI( topLeft, bottomRight ), *(Fixture->GetTexture()), copy );
	}
}

const Vei2& Grid::Cell::GetLocation() const
{
	return Location;
}

void Grid::Cell::SetLocation( const Vei2& location )
{
	Location = location;
}
