#include "Editor.h"

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
		MouseInf.LMouseButtonGridLocation = Vei2( -1, -1 );
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

	if ( MapGrid.IsOnGrid( MouseInf.HoverGridLocation ) )
	{
		MapGrid.HighlightCell( MouseInf.HoverGridLocation, GetCellHoverHighlightColour(), Editor::CellHoverOpacity, gfx );
	}
}

void Editor::CycleMouseLClickMode()
{
	if ( SelectionMode )
	{
		// Don't cycle through while we're in forced selection mode, or the user could get confused.
		return;
	}

	MouseLClickMode = (EditMode::MouseLClick)(((int)MouseLClickMode + 1) % (int)EditMode::MouseLClick::EnumOptionsCount);
}

void Editor::DisableSelectionMode()
{
	SelectionMode = false;
}

void Editor::EnableSelectionMode()
{
	SelectionMode = true;
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
	if ( SelectionMode ) // Forced / overriding selection mode
	{
		return EditMode::MouseLClick::Select;
	}

	return MouseLClickMode;
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

	if ( GetMouseLClickMode() == EditMode::MouseLClick::Insert )
	{
		MapGrid.Fill( gridLocation, Colors::White );
	}
}

void Editor::MouseRPress( const Vei2 & screenLocation )
{
	MapGrid.ClearCell( MapGrid.ScreenToGrid( screenLocation ) );
}
