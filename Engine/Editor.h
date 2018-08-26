#pragma once

#define NOMINMAX

#include <vector>

#include "EditConstants.h"
#include "EditTool.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Grid.h"
#include "Menu.h"
#include "Mouse.h"
#include "Vec2.h"

class EditTool_MouseButton;
class EditTool_MouseButton_Insert;
class MapFixture;

class Editor
{
public:
	static constexpr char EditTool_MouseButton_Left[] = "EditTool_MouseButton_Left";

	class EditTool_MouseButtonCallBack : public SelectedCallBack
	{
	public:
		EditTool_MouseButtonCallBack( Editor* const editor, EditTool_MouseButton* const tool )
			:
			_Editor( editor ),
			Tool( tool )
		{
		}

		virtual void Execute() const override = 0;

	protected:
		Editor* GetEditor() const;
		EditTool_MouseButton* GetTool() const;

	private:
		Editor* const _Editor = nullptr;
		EditTool_MouseButton* const Tool = nullptr;
	};

	class EditTool_MouseButtonLCallBack : public EditTool_MouseButtonCallBack
	{
	public:
		EditTool_MouseButtonLCallBack( Editor* const editor, EditTool_MouseButton* const tool )
			:
			EditTool_MouseButtonCallBack( editor, tool )
		{
		}

		void Execute() const override;
	};

	class EditTool_MouseButton_InsertLCallBack : public EditTool_MouseButtonLCallBack
	{
	public:
		EditTool_MouseButton_InsertLCallBack( Editor* const editor, EditTool_MouseButton* const tool, const MapFixture* const fixture )
			:
			EditTool_MouseButtonLCallBack( editor, tool ),
			Fixture( fixture )
		{
		}

		void Execute() const override;

	private:
		Editor* const _Editor = nullptr;
		const MapFixture* Fixture = nullptr;
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
	const bool GetControlModeEnabled() const;
	Grid& GetMapGrid();
	MouseInfo& GetMouseInfo();
	RectI GetSelectionRectangle( const Vei2& gridLocation ) const;
	const bool GetShiftModeEnabled() const;
	const EditToolBox& GetToolBox() const;

private:
	void CycleMouseLClickMode();
	void DisableSelectionMode();
	void EnableSelectionMode();
	void MouseLDrag( const Vei2& gridLocation );
	void MouseLPress( Mouse::Event& me );
	void MouseLRelease( Mouse::Event& me );
	void MouseMDrag( Mouse::Event& me );
	void MouseMove( Mouse::Event& me );
	void MouseMPress( Mouse::Event& me );
	void MouseRPress( Mouse::Event& me );
	void MouseWheel( Mouse::Event& me );
	void SetControlKeys( const unsigned char c, const bool enabled );
	void SetMouseLButtonTool( EditTool_MouseButton* const tool );

	EditTool_MouseButton* MouseLButtonTool = nullptr;
	EditTool_MouseButton* MouseLButtonToolSelectionOverrideParked = nullptr;

	EditConstants::MouseLClickMode MouseLClickMode = EditConstants::MouseLClickMode::None;

	std::unique_ptr<EditToolBox> ToolBox = nullptr;

	MouseInfo MouseInf;
	bool ControlModeEnabled = false;
	bool ShiftModeEnabled = false;
	bool AppendSelection = false;

	Grid MapGrid;
};