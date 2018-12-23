#pragma once

#include <string>

#include "Grid.h"

class Map
{
public:
	Map();
	Map( std::string name );

	const Grid& GetGrid() const;
	Grid& GetGrid();
	const std::string GetName() const;
	void SetName( std::string name );

private:
	std::string Name;
	Grid _Grid;
};