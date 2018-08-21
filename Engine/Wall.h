#pragma once

#include "MapFixture.h"

class Surface;

class Wall : MapFixture
{
public:
	Wall( const Surface& texture );

	const Surface& GetTexture() const;

private:
	const Surface& Texture;
};