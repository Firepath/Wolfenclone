#include "MapFixture.h"

const Surface* MapFixture::GetTexture() const
{
	return Texture;
}

MapFixture::MapFixture( const Surface * const texture )
	:
	Texture( texture )
{
}
