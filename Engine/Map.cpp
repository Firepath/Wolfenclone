#include "Map.h"
#include "Vec2.h"

Map::Map()
	:
	Map( "" )
{
}

Map::Map( std::string name )
	:
	Name( name ),
	_Grid( 256, 256, Vec2( 0.0f, 0.0f ) )
{
}

const Grid& Map::GetGrid() const
{
	return _Grid;
}

Grid& Map::GetGrid()
{
	return _Grid;

	// If this were non-trivial...
	//return const_cast<Grid&>(static_cast<const Map*>(this)->GetGrid());
}

const std::string Map::GetName() const
{
	return Name;
}

void Map::SetName( std::string name )
{
	Name = name;
}
