#pragma once

#include "MapFixture.h"
#include "Surface.h"

class Wall : MapFixture
{
public:
	Wall( const Surface& texture );

	const Surface& GetTexture() const;

private:
	const Surface& Texture;
};