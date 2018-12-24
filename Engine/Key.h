#pragma once

#include "Pickup.h"

class Key : public Pickup
{
public:
	Key( const std::string name, const std::string type, const Surface* const texture );
};