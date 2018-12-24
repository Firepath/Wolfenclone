#pragma once

#include "Decoration.h"

class Pickup : public Decoration
{
public:
	Pickup( const std::string name, const std::string type, const Surface* const texture );
};