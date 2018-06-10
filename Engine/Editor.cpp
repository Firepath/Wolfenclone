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
			EnableSingleSelectionMode();
			break;
		case VK_SHIFT:
			EnableSelectionMode();
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
			DisableSingleSelectionMode();
			break;
		case VK_SHIFT:
			DisableSelectionMode();
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

	for ( Vei2 gridLocation : TemporarySelectedCells )
	{
		if ( !VectorExtension::Contains( SelectedCells, gridLocation ) ) // Already drawn (above)
		{
			MapGrid.HighlightCell( gridLocation, Editor::SelectModeHoverColour, Editor::CellHoverOpacity, gfx );
		}
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

void Editor::DisableSingleSelectionMode()
{
	SelectionMode = EditMode::Selection::Rectangle;
}

void Editor::DisableSelectionMode()
{
	if ( MouseLClickMode == EditMode::MouseLClick::Select ) // Already in select mode, this can toggle appending
	{
		AppendSelection = false;
	}

	SelectionModeOverride = false;
}

void Editor::EnableSingleSelectionMode()
{
	SelectionMode = EditMode::Selection::Single;
}

void Editor::EnableSelectionMode()
{
	if ( MouseLClickMode == EditMode::MouseLClick::Select ) // Already in select mode, this can toggle appending
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

		if ( !AppendSelection )
		{
			SelectedCells.clear();
		}
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

	// Move temporary selection into "permanent" selection
	for ( Vei2 location : TemporarySelectedCells )
	{
		if ( !VectorExtension::Contains( SelectedCells, location ) )
		{
			SelectedCells.push_back( location );
		}
	}

	// remove from the selection list any cells that are empty
	auto newEnd = std::remove_if( SelectedCells.begin(), SelectedCells.end(), [this]( Vei2 gridLocation ) { return MapGrid.GetCell( gridLocation ).IsEmpty(); } );
	SelectedCells.erase( newEnd, SelectedCells.end() );

	TemporarySelectedCells.clear();
}

void Editor::MouseRPress( const Vei2 & screenLocation )
{
	const Vei2 gridLocation = MapGrid.ScreenToGrid( screenLocation );
	MapGrid.ClearCell( gridLocation );

	auto newEnd = std::remove_if( SelectedCells.begin(), SelectedCells.end(), [&gridLocation]( Vei2 selectedLocation ) { return gridLocation == selectedLocation; } );
	SelectedCells.erase( newEnd, SelectedCells.end() );
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

		TemporarySelectedCells.clear();

		Vei2 topLeft( std::min( startLocation.x, gridLocation.x ), std::min( startLocation.y, gridLocation.y ) );
		Vei2 bottomRight( std::max( startLocation.x, gridLocation.x ), std::max( startLocation.y, gridLocation.y ) );

		TemporarySelectedCells.reserve( (bottomRight.x - topLeft.x) * (bottomRight.y - topLeft.y) );

		for ( int x = topLeft.x; x <= bottomRight.x; x++ )
		{
			for ( int y = topLeft.y; y <= bottomRight.y; y++ )
			{
				TemporarySelectedCells.push_back( Vei2( x, y ) );
			}
		}
	}
		break;
	case EditMode::Selection::Single:
		if ( !VectorExtension::Contains( TemporarySelectedCells, gridLocation ) )
		{
			TemporarySelectedCells.push_back( gridLocation );
		}
		break;
	default:
		break;
	}
}
