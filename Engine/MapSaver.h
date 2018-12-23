#pragma once

#include "Grid.h"
#include "Saver.h"

class Map;
class Grid::Cell;

class MapSaver : public Saver
{
public:
	MapSaver( const Map& map );

private:
	void ProcessFile( const std::string& filename ) override;
	void WriteLine( const std::string& line, std::ofstream& out ) override;
	void WriteMapGridCell( const Grid::Cell& cell, std::ofstream& out ) const;
	void WriteMapGridCells( std::ofstream& out ) const;

	const Map& _Map;
};