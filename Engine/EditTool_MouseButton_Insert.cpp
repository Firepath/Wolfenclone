#include "EditConstants.h"
#include "Editor.h"
#include "EditTool.h"

void EditTool_MouseButton_Insert::ButtonPressed()
{
	EditTool_MouseButton::ButtonPressed();

	Editor& editor = GetEditor();

	if ( Fixture != nullptr )
	{
		Editor::MouseInfo& info = editor.GetMouseInfo();
		Grid& mapGrid = editor.GetMap().GetGrid();
		if ( editor.GetControlModeEnabled() )
		{
			mapGrid.FillSelectedCells( Fixture );
		}
		else
		{
			editor.GetMap().GetGrid().Fill( info.HoverGridLocation, Fixture );
		}
	}

	editor.GetMap().GetGrid().ClearSelectedCells();
}

void EditTool_MouseButton_Insert::ButtonReleased()
{
	EditTool_MouseButton::ButtonReleased();
}

const MapFixture* EditTool_MouseButton_Insert::GetFixture() const
{
	return Fixture;
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
