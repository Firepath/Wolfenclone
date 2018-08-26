#include "Editor.h"
#include "EditTool.h"

void EditTool_MouseButton::ButtonPressed()
{
	ButtonIsPressed = true;
}

void EditTool_MouseButton::ButtonReleased()
{
	ButtonIsPressed = false;

	Editor& editor = GetEditor();
	Grid& mapGrid = editor.GetMapGrid();
	mapGrid.SetTemporarySelectedToSelected();
	mapGrid.SetTemporaryMovedToMoved();
}
