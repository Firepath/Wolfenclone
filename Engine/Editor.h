#pragma once

#include "Graphics.h"
#include "Map.h"
#include "Mouse.h"
#include "Vec2.h"

class Editor
{
public:
	Editor();

	void DoMouseEvents( const Mouse::Event& me );
	void Draw( Graphics& gfx );

private:
	Map MapGrid;
};