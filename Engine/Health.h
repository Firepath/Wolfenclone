#pragma once

#include "Pickup.h"

class Health : public Pickup
{
public:
	Health( const std::string name, const std::string type, const Surface* const texture );
};