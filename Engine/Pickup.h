#pragma once

#include "MapFixture.h"

class Pickup : public MapFixture
{
public:
	Pickup( const Surface* const texture );

	void Draw( const RectI area, Graphics& gfx ) const override;
};