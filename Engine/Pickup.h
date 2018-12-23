#pragma once

#include "Fixture.h"

class Pickup : public Fixture
{
public:
	Pickup( const std::string name, const std::string type, const Surface* const texture );

	void Draw( const RectI area, Graphics& gfx ) const override;
};