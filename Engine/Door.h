#pragma once

#include "Fixture.h"

class Door : public Fixture
{
public:
	Door( const std::string name, const std::string type, const Surface* const texture );

	void Draw( const RectI area, Graphics& gfx ) const override;
};