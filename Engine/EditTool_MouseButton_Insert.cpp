#include "EditConstants.h"
#include "Editor.h"
#include "EditTool.h"

void EditTool_MouseButton_Insert::ButtonPressed()
{
	EditTool_MouseButton::ButtonPressed();

	Editor& editor = GetEditor();

	editor.GetMapGrid().ClearSelectedCells();

	if ( Fixture != nullptr )
	{
		Editor::MouseInfo& info = editor.GetMouseInfo();
		editor.GetMapGrid().Fill( info.HoverGridLocation, Fixture );
	}
}

void EditTool_MouseButton_Insert::ButtonReleased()
{
	EditTool_MouseButton::ButtonReleased();
}

Color EditTool_MouseButton_Insert::GetToolColour() const
{
	return EditConstants::CellEditing::InsertModeHoverColour;
}

void EditTool_MouseButton_Insert::MouseMoved()
{
}

void EditTool_MouseButton_Insert::SetFixture( const MapFixture* fixture )
{
	Fixture = fixture;
}
