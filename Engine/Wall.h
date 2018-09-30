#pragma once

#include "MapFixture.h"

class Wall : public MapFixture
{
public:
	Wall( const Surface* const texture );

	void Draw( const RectI area, Graphics& gfx ) const override;
};