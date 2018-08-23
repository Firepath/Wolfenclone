#pragma once

#define NOMINMAX

#include <vector>

#include "EditConstants.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Grid.h"
#include "Menu.h"
#include "Mouse.h"
#include "Vec2.h"

class Surface;

class Editor
{
public:
	class LeftMouseClickEditModeCallBack : public SelectedCallBack
	{
	public:
		LeftMouseClickEditModeCallBack( Editor* const editor, EditConstants::MouseLClickMode mode, const Surface* const surface );

		void Execute() const override;

	private:
		Editor* const _Editor;
		EditConstants::MouseLClickMode Mode;
		const Surface* const _Surface;
	};

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

	void DoKeyboardEvents( Keyboard::Event& ke );
	void DoMouseEvents( Mouse::Event& me );
	void Draw( Graphics& gfx );
	const Color GetCellHoverHighlightColour( const EditConstants::MouseLClickMode mode ) const;

private:
	void CycleMouseLClickMode();
	void DisableSingleSelectionMode();
	void DisableSelectionMode();
	void EnableSingleSelectionMode();
	void EnableSelectionMode();
	const Color GetCellHoverHighlightColour() const;
	const EditConstants::MouseLClickMode GetMouseLClickMode() const;
	const EditConstants::SelectionMode GetSelectionMode() const;
	void MouseLPress( const Vei2& screenLocation );
	void MouseLRelease();
	void MouseRPress( const Vei2& screenLocation );
	void SelectCell( const Vei2& gridLocation );
	void SetInsertSurface( const Surface* const surface ); // will be changed to be a MapFixture eventually
	void SetMouseLClickMode( EditConstants::MouseLClickMode mode );

	//Surface GreyWallSurface;// = Surface( "Textures\\greystone.bmp" );
	const Surface* InsertSurface = nullptr;

	EditConstants::MouseLClickMode MouseLClickMode = EditConstants::MouseLClickMode::None;
	EditConstants::SelectionMode SelectionMode = EditConstants::SelectionMode::Rectangle;

	MouseInfo MouseInf;
	bool SelectionModeOverride = false;
	bool AppendSelection = false;

	Grid MapGrid;
};