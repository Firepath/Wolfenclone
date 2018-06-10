#pragma once

#include "EditMode.h"
#include "Graphics.h"
#include "Map.h"
#include "Mouse.h"
#include "Vec2.h"

class Editor
{
public:
	struct MouseInfo
	{
		// In Grid Coordinates
		Vei2 HoverGridLocation = Vei2( -1, -1 );
		Vei2 LMouseButtonGridLocation = Vei2( -1, -1 );

		// In Screen Coordinates
		Vei2 MMouseButtonLocation = Vei2( -1, -1 );
	};

	Editor();

	void DoMouseEvents( const Mouse::Event& me );
	void Draw( Graphics& gfx );

private:
	void MouseLPress( const Vei2& screenLocation );

	EditMode::MouseLClick MouseLClickMode = EditMode::MouseLClick::None;

	MouseInfo MouseInf;

	Map MapGrid;
};