#pragma once

#include "EditMode.h"
#include "Graphics.h"
#include "Keyboard.h"
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

	void DoKeyboardEvents( const Keyboard::Event& ke );
	void DoMouseEvents( const Mouse::Event& me );
	void Draw( Graphics& gfx );

private:
	static constexpr Color InactiveModeHoverColour = Colors::MediumGray;
	static constexpr Color InsertModeHoverColour = Colors::Green;
	static constexpr Color SelectModeHoverColour = Colors::Yellow;
	static constexpr float CellHoverOpacity = 50.0f;

	void CycleMouseLClickMode();
	void DisableSelectionMode();
	void EnableSelectionMode();
	const Color GetCellHoverHighlightColour() const;
	const EditMode::MouseLClick GetMouseLClickMode() const;
	void MouseLPress( const Vei2& screenLocation );
	void MouseRPress( const Vei2& screenLocation );

	EditMode::MouseLClick MouseLClickMode = EditMode::MouseLClick::None;

	MouseInfo MouseInf;
	bool SelectionMode = false;

	Map MapGrid;
};