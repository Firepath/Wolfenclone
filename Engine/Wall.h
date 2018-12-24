#pragma once

#include "Fixture.h"

class Wall : public Fixture
{
public:
	Wall( const std::string name, const std::string type, const Surface* const texture );

	void Draw( const RectI area, Graphics& gfx ) const override;
	const bool Enclosing() const override;
};