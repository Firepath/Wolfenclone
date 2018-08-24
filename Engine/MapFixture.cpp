#include "MapFixture.h"

const Surface* MapFixture::GetTexture() const
{
	return Texture;
}

void MapFixture::SetTexture( const Surface* const texture )
{
	Texture = texture;
}
