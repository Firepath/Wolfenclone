#include "EditConstants.h"
#include "Editor.h"
#include "EditTool.h"

void EditTool_MouseButton_Delete::ButtonPressed()
{
	EditTool_MouseButton::ButtonPressed();

	Editor& editor = GetEditor();
	const Editor::MouseInfo& info = editor.GetMouseInfo();
	Grid& mapGrid = editor.GetMapGrid();

	if ( editor.GetControlModeEnabled() )
	{
		mapGrid.DeleteSelectedCells();
	}
	else
	{
		mapGrid.DeleteCell( info.HoverGridLocation, true );
	}
}

void EditTool_MouseButton_Delete::ButtonReleased()
{
	EditTool_MouseButton::ButtonReleased();
}

Color EditTool_MouseButton_Delete::GetToolColour() const
{
	return EditConstants::CellEditing::DeleteModeHoverColour;
}

void EditTool_MouseButton_Delete::MouseMoved()
{
}
