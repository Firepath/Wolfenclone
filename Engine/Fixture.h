#pragma once

#include <string>

#include "Graphics.h"
#include "Rect.h"

class Player;
class Surface;

class Fixture
{
public:
	Fixture( const std::string name, const std::string type, const Surface* const texture );

	~Fixture() = default;

	virtual const bool BlocksMovement() const;
	virtual void Draw( const RectI area, Graphics& gfx ) const;
	virtual const bool Enclosing() const;
	const std::string GetName() const;
	const std::string GetType() const;
	const Surface* GetTexture() const;
	virtual void InteractionEffect();
	virtual const bool PlayerEffect( Player& player ) const;

protected:
	const std::string Name;
	const std::string Type;
	const Surface* const Texture = nullptr;
};