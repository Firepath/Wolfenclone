#include "Wall.h"

Wall::Wall( const Surface & texture )
	:
	Texture( texture )
{
}

const Surface & Wall::GetTexture() const
{
	return Texture;
}
