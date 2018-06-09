#pragma once

#include "EditMode.h"
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
	EditMode::MouseLClick MouseLClickMode = EditMode::MouseLClick::None;

	Map MapGrid;
};