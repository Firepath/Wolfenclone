#include "MapSaver.h"

#include <string>

#include "Grid.h"
#include "Map.h"
#include "MapFixture.h"
#include "Vec2.h"

MapSaver::MapSaver( const Map& map )
	:
	_Map( map )
{
}

void MapSaver::ProcessFile( const std::string& filename )
{
	Mode = FileIOConstants::IOMode::None;
		
	std::ofstream out( filename );
	if ( !out.good() )
	{
		// TODO: More than just return.
		return;
	}

	Mode = FileIOConstants::IOMode::Map_Name;
	WriteIOModeTag( out );
	WriteLine( _Map.GetName(), out );

	Mode = FileIOConstants::IOMode::Map_Cell_Content;
	WriteIOModeTag( out );
	WriteMapGridCells( out );

	Mode = FileIOConstants::IOMode::None;
}

void MapSaver::WriteLine( const std::string& line, std::ofstream& out )
{
	out << line << std::endl;
}

void MapSaver::WriteMapGridCell( const Grid::Cell& cell, std::ofstream& out ) const
{
	const Vei2 location = cell.GetLocation();
	const MapFixture& fixture = *(cell.GetFixture());

	out << location.x << "," << location.y << " " << fixture.GetType() << " " << fixture.GetName() << std::endl;
}

void MapSaver::WriteMapGridCells( std::ofstream & out ) const
{
	const Grid& grid = _Map.GetGrid();
	const Vei2 gridSize = grid.GetSize();
	for ( int y = 0; y < gridSize.y; y++ )
	{
		for ( int x = 0; x < gridSize.x; x++ )
		{
			const Vei2 cellLocation( x, y );
			if ( grid.IsCellOccupied( cellLocation ) )
			{
				WriteMapGridCell( grid.GetCell( cellLocation ), out );
			}
		}
	}
}
