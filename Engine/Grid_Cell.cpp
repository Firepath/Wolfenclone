#include "Grid.h"

Grid::Cell::Cell( const Vei2& location, const MapFixture* const fixture )
	:
	Location( location ),
	Fixture( fixture )
{
}

const MapFixture* Grid::Cell::GetFixture() const
{
	return Fixture;
}

const Vei2& Grid::Cell::GetLocation() const
{
	return Location;
}

void Grid::Cell::SetLocation( const Vei2& location )
{
	Location = location;
}
