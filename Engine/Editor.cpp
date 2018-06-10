#include "Editor.h"
#include "VectorExtensions.h"

Editor::Editor()
	:
	MapGrid( 180, 100, Vec2( 0.0f, 0.0f ) )
{
}

void Editor::DoKeyboardEvents( const Keyboard::Event & ke )
{
	const char c = ke.GetCode();
	if ( ke.IsPress() )
	{
		switch ( c )
		{
		case VK_CONTROL:
			EnableSelectionMode();
			break;
		case VK_SHIFT:
			ToggleSelectionMode();
			break;
		case VK_TAB:
			CycleMouseLClickMode();
		default:
			break;
		}
	}
	else if ( ke.IsRelease() )
	{
		switch ( c )
		{
		case VK_CONTROL:
			DisableSelectionMode();
			break;
		case VK_SHIFT:
			ToggleSelectionMode();
			break;
		default:
			break;
		}
	}
}

void Editor::DoMouseEvents( const Mouse::Event & me )
{
	Mouse::Event::Type meType = me.GetType();

	switch ( meType )
	{
	case Mouse::Event::Type::LPress:
		MouseLPress( me.GetPos() );
		break;
	case Mouse::Event::Type::LRelease:
		MouseLRelease();
		break;
	case Mouse::Event::Type::Move:
	{
		const Vei2 mousePos = me.GetPos();

		MouseInf.HoverGridLocation = MapGrid.ScreenToGrid( mousePos );

		if ( me.LeftIsPressed() )
		{
			MouseLPress( mousePos );
		}

		if ( me.MiddleIsPressed() )
		{
			const Vei2 delta = mousePos - MouseInf.MMouseButtonLocation;
			MouseInf.MMouseButtonLocation = mousePos;
			MapGrid.Move( (Vec2)delta );
		}

		if ( me.RightIsPressed() )
		{
			MouseRPress( mousePos );
		}
	}
	break;
	case Mouse::Event::Type::MPress:
		MouseInf.MMouseButtonLocation = me.GetPos();
		break;
	case Mouse::Event::Type::RPress:
		MouseRPress( me.GetPos() );
		break;
	case Mouse::Event::Type::WheelUp:
	case Mouse::Event::Type::WheelDown:
		MapGrid.Zoom( (Vec2)me.GetPos(), meType == Mouse::Event::Type::WheelUp );
		break;
	default:
		break;
	}
}

void Editor::Draw( Graphics & gfx )
{
	MapGrid.Draw( gfx );

	for ( Vei2 gridLocation : SelectedCells )
	{
		MapGrid.HighlightCell( gridLocation, Editor::SelectModeHoverColour, Editor::CellHoverOpacity, gfx );
	}

	if ( MapGrid.IsOnGrid( MouseInf.HoverGridLocation ) )
	{
		MapGrid.HighlightCell( MouseInf.HoverGridLocation, GetCellHoverHighlightColour(), Editor::CellHoverOpacity, gfx );
	}
}

void Editor::CycleMouseLClickMode()
{
	if ( SelectionModeOverride )
	{
		// Don't cycle through while we're in forced selection mode, or the user could get confused.
		return;
	}

	MouseLClickMode = (EditMode::MouseLClick)(((int)MouseLClickMode + 1) % (int)EditMode::MouseLClick::EnumOptionsCount);
}

void Editor::DisableSelectionMode()
{
	SelectionModeOverride = false;
}

void Editor::EnableSelectionMode()
{
	SelectionModeOverride = true;
}

const Color Editor::GetCellHoverHighlightColour() const
{
	Color colour = Editor::InactiveModeHoverColour;

	switch ( GetMouseLClickMode() )
	{
	case EditMode::MouseLClick::Insert:
		colour = Editor::InsertModeHoverColour;
		break;
	case EditMode::MouseLClick::Select:
		colour = Editor::SelectModeHoverColour;
		break;
	default:
		break;
	}

	return colour;
}

const EditMode::MouseLClick Editor::GetMouseLClickMode() const
{
	if ( SelectionModeOverride ) // Forced / overriding selection mode
	{
		return EditMode::MouseLClick::Select;
	}

	return MouseLClickMode;
}

const EditMode::Selection Editor::GetSelectionMode() const
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
	}

	switch ( GetMouseLClickMode() )
	{
	case EditMode::MouseLClick::Insert:
		MapGrid.Fill( gridLocation, Colors::White );
		break;
	case EditMode::MouseLClick::Select:
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
	SelectedCells.clear();
}

void Editor::MouseRPress( const Vei2 & screenLocation )
{
	MapGrid.ClearCell( MapGrid.ScreenToGrid( screenLocation ) );
}

void Editor::SelectCell( const Vei2& gridLocation )
{
	if ( !MapGrid.IsOnGrid( gridLocation ) )
	{
		return;
	}

	switch ( GetSelectionMode() )
	{
	case EditMode::Selection::Rectangle:
	{
		Vei2 startLocation = MouseInf.LMouseButtonGridLocationAtLPress;
		if ( !MapGrid.IsOnGrid( startLocation ) )
		{
			return;
		}

		Vei2 topLeft( std::min( startLocation.x, gridLocation.x ), std::min( startLocation.y, gridLocation.y ) );
		Vei2 bottomRight( std::max( startLocation.x, gridLocation.x ), std::max( startLocation.y, gridLocation.y ) );

		SelectedCells.clear();

		for ( int x = topLeft.x; x <= bottomRight.x; x++ )
		{
			for ( int y = topLeft.y; y <= bottomRight.y; y++ )
			{
				SelectedCells.push_back( Vei2( x, y ) );
			}
		}
	}
		break;
	case EditMode::Selection::Single:
		if ( !VectorExtension::Contains( SelectedCells, gridLocation ) )
		{
			SelectedCells.push_back( gridLocation );
		}
		break;
	default:
		break;
	}
}

void Editor::ToggleSelectionMode()
{
	if ( SelectionMode == EditMode::Selection::Rectangle )
	{
		SelectionMode = EditMode::Selection::Single;
	}
	else
	{
		SelectionMode = EditMode::Selection::Rectangle;
	}
}
