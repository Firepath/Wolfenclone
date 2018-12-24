#include "Weapon.h"

Weapon::Weapon( const std::string name, const std::string type, const Surface * const texture )
	:
	Pickup( name, type, texture )
{
}
