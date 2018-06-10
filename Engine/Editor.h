#pragma once

#define NOMINMAX

#include <vector>

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
		Vei2 LMouseButtonGridLocationAtLPress = Vei2( -1, -1 );

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
	void DisableSingleSelectionMode();
	void DisableSelectionMode();
	void EnableSingleSelectionMode();
	void EnableSelectionMode();
	const Color GetCellHoverHighlightColour() const;
	const EditMode::MouseLClick GetMouseLClickMode() const;
	const EditMode::Selection GetSelectionMode() const;
	void MouseLPress( const Vei2& screenLocation );
	void MouseLRelease();
	void MouseRPress( const Vei2& screenLocation );
	void SelectCell( const Vei2& gridLocation );

	EditMode::MouseLClick MouseLClickMode = EditMode::MouseLClick::None;
	EditMode::Selection SelectionMode = EditMode::Selection::Rectangle;

	MouseInfo MouseInf;
	bool SelectionModeOverride = false;
	bool AppendSelection = false;
	std::vector<Vei2> SelectedCells;
	std::vector<Vei2> TemporarySelectedCells;

	Map MapGrid;
};