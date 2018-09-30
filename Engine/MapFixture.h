#pragma once

#include "Graphics.h"
#include "Rect.h"

class Surface;

class MapFixture
{
public:
	MapFixture( const Surface* const texture );

	~MapFixture() = default;

	virtual void Draw( const RectI area, Graphics& gfx ) const = 0;
	const Surface* GetTexture() const;

protected:
	const Surface* const Texture = nullptr;
};