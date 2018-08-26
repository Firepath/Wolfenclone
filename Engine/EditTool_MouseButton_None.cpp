#include "EditConstants.h"
#include "Editor.h"
#include "EditTool.h"

void EditTool_MouseButton_None::ButtonPressed()
{
	EditTool_MouseButton::ButtonPressed();
	
	GetEditor().GetMapGrid().ClearSelectedCells();
}

void EditTool_MouseButton_None::ButtonReleased()
{
	EditTool_MouseButton::ButtonReleased();
}

Color EditTool_MouseButton_None::GetToolColour() const
{
	return EditConstants::CellEditing::InactiveModeHoverColour;
}

void EditTool_MouseButton_None::MouseMoved()
{
}
