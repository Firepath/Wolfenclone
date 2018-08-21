#include "Editor.h"
#include "VectorExtensions.h"

Editor::Editor()
	:
	MapGrid( 256, 256, Vec2( 0.0f, 0.0f ) )
{
}

void Editor::DoKeyboardEvents( Keyboard::Event& ke )
{
	// Something else has already handled this
	if ( ke.IsHandled() )
	{
		return;
	}

	const unsigned char c = ke.GetCode();
	if ( ke.IsPress() )
	{
		switch ( c )
		{
		case '`':
		case '~':
			MapGrid.ToggleGridDrawing();
			ke.SetHandled( true );
			break;
		case VK_BACK:
		case VK_DELETE:
			MapGrid.DeleteSelectedCells();
			ke.SetHandled( true );
			break;
		case VK_CONTROL:
			EnableSingleSelectionMode();
			ke.SetHandled( true );
			break;
		case VK_ESCAPE:
			MapGrid.ClearSelectedCells();
			ke.SetHandled( true );
			break;
		case VK_SHIFT:
			EnableSelectionMode();
			ke.SetHandled( true );
			break;
		case VK_TAB:
			CycleMouseLClickMode();
			ke.SetHandled( true );
		default:
			break;
		}
	}
	else if ( ke.IsRelease() )
	{
		switch ( c )
		{
		case VK_CONTROL:
			DisableSingleSelectionMode();
			ke.SetHandled( true );
			break;
		case VK_SHIFT:
			DisableSelectionMode();
			ke.SetHandled( true );
			break;
		default:
			break;
		}
	}
}

void Editor::DoMouseEvents( Mouse::Event& me )
{
	// Something else has already handled this
	if ( me.IsHandled() )
	{
		return;
	}

	Mouse::Event::Type meType = me.GetType();

	switch ( meType )
	{
	case Mouse::Event::Type::LPress:
		MouseLPress( me.GetPos() );
		me.SetHandled( true );
		break;
	case Mouse::Event::Type::LRelease:
		MouseLRelease();
		me.SetHandled( true );
		break;
	case Mouse::Event::Type::Move:
	{
		const Vei2 mousePos = me.GetPos();

		MouseInf.HoverGridLocation = MapGrid.ScreenToGrid( mousePos );

		if ( me.LeftIsPressed() )
		{
			MouseLPress( mousePos );
			me.SetHandled( true );
		}

		if ( me.MiddleIsPressed() )
		{
			const Vei2 delta = mousePos - MouseInf.MMouseButtonLocation;
			MouseInf.MMouseButtonLocation = mousePos;
			MapGrid.Move( (Vec2)delta );
			me.SetHandled( true );
		}

		if ( me.RightIsPressed() )
		{
			MouseRPress( mousePos );
			me.SetHandled( true );
		}
	}
	break;
	case Mouse::Event::Type::MPress:
		MouseInf.MMouseButtonLocation = me.GetPos();
		me.SetHandled( true );
		break;
	case Mouse::Event::Type::RPress:
		MouseRPress( me.GetPos() );
		me.SetHandled( true );
		break;
	case Mouse::Event::Type::WheelUp:
	case Mouse::Event::Type::WheelDown:
		MapGrid.Zoom( (Vec2)me.GetPos(), meType == Mouse::Event::Type::WheelUp );
		me.SetHandled( true );
		break;
	default:
		break;
	}
}

void Editor::Draw( Graphics& gfx )
{
	MapGrid.Draw( gfx );

	// Highlight currently-hovered cell
	if ( MapGrid.IsOnGrid( MouseInf.HoverGridLocation ) )
	{
		MapGrid.HighlightCell( MouseInf.HoverGridLocation, GetCellHoverHighlightColour(), EditConstants::CellSelection::CellHoverOpacity, true, gfx );
	}
}

void Editor::CycleMouseLClickMode()
{
	if ( SelectionModeOverride )
	{
		// Don't cycle through while we're in forced selection mode, or the user could get confused.
		return;
	}

	MouseLClickMode = (EditConstants::MouseLClickMode)(((int)MouseLClickMode + 1) % (int)EditConstants::MouseLClickMode::EnumOptionsCount);
}

void Editor::DisableSingleSelectionMode()
{
	SelectionMode = EditConstants::SelectionMode::Rectangle;
}

void Editor::DisableSelectionMode()
{
	if ( MouseLClickMode == EditConstants::MouseLClickMode::Select ) // Already in select mode, this can toggle appending
	{
		AppendSelection = false;
	}

	SelectionModeOverride = false;
}

void Editor::EnableSingleSelectionMode()
{
	SelectionMode = EditConstants::SelectionMode::Single;
}

void Editor::EnableSelectionMode()
{
	if ( MapGrid.IsOnGrid( MouseInf.LMouseButtonGridLocationAtLPress ) )
	{
		// Already have left mouse button down, only do toggling beforehand
		return;
	}

	if ( MouseLClickMode == EditConstants::MouseLClickMode::Select ) // Already in select mode, this can toggle appending
	{
		AppendSelection = true;
	}
	else
	{
		SelectionModeOverride = true;
	}
}

const Color Editor::GetCellHoverHighlightColour() const
{
	Color colour = EditConstants::CellSelection::InactiveModeHoverColour;

	switch ( GetMouseLClickMode() )
	{
	case EditConstants::MouseLClickMode::Insert:
		colour = EditConstants::CellSelection::InsertModeHoverColour;
		break;
	case EditConstants::MouseLClickMode::Select:
		colour = EditConstants::CellSelection::SelectModeHoverColour;
		break;
	default:
		break;
	}

	return colour;
}

const EditConstants::MouseLClickMode Editor::GetMouseLClickMode() const
{
	if ( SelectionModeOverride ) // Forced / overriding selection mode
	{
		return EditConstants::MouseLClickMode::Select;
	}

	return MouseLClickMode;
}

const EditConstants::SelectionMode Editor::GetSelectionMode() const
{
	return SelectionMode;
}

void Editor::MouseLPress( const Vei2& screenLocation )
{
	const Vei2 gridLocation = MapGrid.ScreenToGrid( screenLocation );
	if ( !MapGrid.IsOnGrid( gridLocation ) )
	{
		return;
	}

	if ( gridLocation == MouseInf.LMouseButtonGridLocation )
	{
		// Don't want to be automatically clicking the same thing more than once
		return;
	}

	MouseInf.LMouseButtonGridLocation = gridLocation;

	if ( !MapGrid.IsOnGrid( MouseInf.LMouseButtonGridLocationAtLPress ) )
	{
		// Set this location only when first pressing the left mouse button down (start press location)
		MouseInf.LMouseButtonGridLocationAtLPress = gridLocation;

		if ( !AppendSelection )
		{
			MapGrid.ClearSelectedCells();
		}
	}

	switch ( GetMouseLClickMode() )
	{
	case EditConstants::MouseLClickMode::Insert:
		MapGrid.Fill( gridLocation, &GreyWallSurface );
		break;
	case EditConstants::MouseLClickMode::Select:
		SelectCell( gridLocation );
		break;
	default:
		break;
	}
}

void Editor::MouseLRelease()
{
	MouseInf.LMouseButtonGridLocation = Vei2( -1, -1 );
	MouseInf.LMouseButtonGridLocationAtLPress = Vei2( -1, -1 );

	MapGrid.SetTemporarySelectedToSelected();
}

void Editor::MouseRPress( const Vei2 & screenLocation )
{
	MapGrid.DeleteCell( MapGrid.ScreenToGrid( screenLocation ) );
}

void Editor::SelectCell( const Vei2& gridLocation )
{
	if ( !MapGrid.IsOnGrid( gridLocation ) )
	{
		return;
	}

	switch ( GetSelectionMode() )
	{
	case EditConstants::SelectionMode::Rectangle:
	{
		Vei2 startLocation = MouseInf.LMouseButtonGridLocationAtLPress;
		if ( !MapGrid.IsOnGrid( startLocation ) )
		{
			return;
		}

		Vei2 topLeft( std::min( startLocation.x, gridLocation.x ), std::min( startLocation.y, gridLocation.y ) );
		Vei2 bottomRight( std::max( startLocation.x, gridLocation.x ), std::max( startLocation.y, gridLocation.y ) );

		MapGrid.TemporarySelectCellsInRectangle( RectI( topLeft, bottomRight ) );
	}
		break;
	case EditConstants::SelectionMode::Single:
		MapGrid.TemporarySelectCell( gridLocation );
		break;
	default:
		break;
	}
}
