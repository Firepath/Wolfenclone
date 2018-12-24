#include "Furniture.h"

Furniture::Furniture( const std::string name, const std::string type, const Surface * const texture )
	:
	Decoration( name, type, texture )
{
}

const bool Furniture::BlocksMovement() const
{
	return true;
}
