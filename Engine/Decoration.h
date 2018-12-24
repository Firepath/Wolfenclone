#pragma once

#include "Fixture.h"

class Decoration : public Fixture
{
public:
	Decoration( const std::string name, const std::string type, const Surface* const texture );

	virtual const bool BlocksMovement() const override;
	virtual void Draw( const RectI area, Graphics& gfx ) const override;
};