#pragma once

#include <string>

#include "Graphics.h"
#include "Rect.h"

class Surface;

class MapFixture
{
public:
	MapFixture( const std::string name, const std::string type, const Surface* const texture );

	~MapFixture() = default;

	virtual void Draw( const RectI area, Graphics& gfx ) const = 0;
	const std::string GetName() const;
	const std::string GetType() const;
	const Surface* GetTexture() const;

protected:
	const std::string Name;
	const std::string Type;
	const Surface* const Texture = nullptr;
};