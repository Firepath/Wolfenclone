#pragma once

#include "Pickup.h"

class Ammo : public Pickup
{
public:
	Ammo( const std::string name, const std::string type, const Surface* const texture );
};