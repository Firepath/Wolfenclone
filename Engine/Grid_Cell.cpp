#include "Grid.h"

Grid::Cell::Cell( const Vei2& location, const Fixture* const fixture )
	:
	Location( location ),
	_Fixture( fixture )
{
}

const Fixture* Grid::Cell::GetFixture() const
{
	return _Fixture;
}

const Vei2& Grid::Cell::GetLocation() const
{
	return Location;
}

void Grid::Cell::SetLocation( const Vei2& location )
{
	Location = location;
}
