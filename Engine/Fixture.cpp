#include "Fixture.h"

Fixture::Fixture( const std::string name, const std::string type, const Surface* const texture )
	:
	Name( name ),
	Type( type ),
	Texture( texture )
{
}

const std::string Fixture::GetName() const
{
	return Name;
}

const std::string Fixture::GetType() const
{
	return Type;
}

const Surface* Fixture::GetTexture() const
{
	return Texture;
}
