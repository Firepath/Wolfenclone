#include "MapFixture.h"

MapFixture::MapFixture( const std::string name, const std::string type, const Surface* const texture )
	:
	Name( name ),
	Type( type ),
	Texture( texture )
{
}

const std::string MapFixture::GetName() const
{
	return Name;
}

const std::string MapFixture::GetType() const
{
	return Type;
}

const Surface* MapFixture::GetTexture() const
{
	return Texture;
}
