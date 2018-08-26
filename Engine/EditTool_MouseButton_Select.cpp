#include "EditConstants.h"
#include "Editor.h"
#include "EditTool.h"

void EditTool_MouseButton_Select::ButtonPressed()
{
	EditTool_MouseButton::ButtonPressed();

	Editor& editor = GetEditor();
	Editor::MouseInfo& info = editor.GetMouseInfo();
	Grid& mapGrid = editor.GetMapGrid();

	if ( !AppendSelection() )
	{
		mapGrid.ClearSelectedCells();
	}

	if ( !mapGrid.IsOnGrid( info.HoverGridLocation ) )
	{
		return;
	}

	switch ( GetSelectionMode() )
	{
	case SelectionMode::Rectangle:
	{
		if ( !mapGrid.IsOnGrid( info.LMouseButtonGridLocationAtLPress ) )
		{
			return;
		}

		const RectI rectangle = editor.GetSelectionRectangle( info.HoverGridLocation );

		mapGrid.TemporarySelectCellsInRectangle( rectangle );
	}
		break;
	case SelectionMode::Single:
		mapGrid.TemporarySelectCell( info.HoverGridLocation );
		break;
	default:
		break;
	}
}

void EditTool_MouseButton_Select::ButtonReleased()
{
	EditTool_MouseButton::ButtonReleased();
}

Color EditTool_MouseButton_Select::GetToolColour() const
{
	return EditConstants::CellEditing::SelectModeHoverColour;
}

void EditTool_MouseButton_Select::MouseMoved()
{
}

const bool EditTool_MouseButton_Select::AppendSelection() const
{
	return GetEditor().GetShiftModeEnabled();
}

const EditTool_MouseButton_Select::SelectionMode EditTool_MouseButton_Select::GetSelectionMode() const
{
	if ( GetEditor().GetControlModeEnabled() )
	{
		return SelectionMode::Single;
	}

	return SelectionMode::Rectangle;
}
