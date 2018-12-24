#include "Fixture.h"

Fixture::Fixture( const std::string name, const std::string type, const Surface* const texture )
	:
	Name( name ),
	Type( type ),
	Texture( texture )
{
}

const bool Fixture::BlocksMovement() const
{
	// Most fixtures will block movement.
	return true;
}

void Fixture::Draw( const RectI area, Graphics & gfx ) const
{
}

const bool Fixture::Enclosing() const
{
	// Most fixtures will not enclose the playable area.
	return false;
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

void Fixture::InteractionEffect()
{
}

const bool Fixture::PlayerEffect( Player& player ) const
{
	return false;
}