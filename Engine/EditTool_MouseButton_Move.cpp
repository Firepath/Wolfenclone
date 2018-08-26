#include "EditConstants.h"
#include "Editor.h"
#include "EditTool.h"

void EditTool_MouseButton_Move::ButtonPressed()
{
	bool buttonIsAlreadyPressed = ButtonIsPressed;
	EditTool_MouseButton::ButtonPressed();

	Editor& editor = GetEditor();
	const Editor::MouseInfo& info = editor.GetMouseInfo();
	Grid& mapGrid = editor.GetMapGrid();
	if ( !buttonIsAlreadyPressed && !mapGrid.HasSelectedCells() )
	{
		mapGrid.TemporarySelectCell( info.HoverGridLocation );
		mapGrid.SetTemporarySelectedToSelected();
	}
}

void EditTool_MouseButton_Move::ButtonReleased()
{
	EditTool_MouseButton::ButtonReleased();
}

Color EditTool_MouseButton_Move::GetToolColour() const
{
	return EditConstants::CellEditing::MoveModeHoverColour;
}

void EditTool_MouseButton_Move::MouseMoved()
{
	Editor& editor = GetEditor();
	if ( !editor.GetMapGrid().HasSelectedCells() )
	{
		return;
	}

	Editor::MouseInfo& info = editor.GetMouseInfo();
	const Vei2 delta = info.HoverGridLocation - info.LMouseButtonGridLocationAtLPress;
	if ( delta.LenSq() == 0 )
	{
		return;
	}

	editor.GetMapGrid().TemporaryMoveSelectedCells( delta );
	info.LMouseButtonGridLocationAtLPress = info.HoverGridLocation;
}
