#pragma once

#include "Pickup.h"

class Weapon : public Pickup
{
public:
	Weapon( const std::string name, const std::string type, const Surface* const texture );
};