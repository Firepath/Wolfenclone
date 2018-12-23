#pragma once

#include "Pickup.h"

class Treasure : public Pickup
{
public:
	Treasure( const std::string name, const std::string type, const Surface* const texture );
};