#include "EditTool.h"
#include "Editor.h"
#include "VectorExtensions.h"

void Editor::EditTool_MouseButtonLCallBack::Execute() const
{
	GetEditor()->SetMouseLButtonTool( GetTool() );
}

Editor* Editor::EditTool_MouseButtonCallBack::GetEditor() const
{
	return _Editor;
}

EditTool_MouseButton* Editor::EditTool_MouseButtonCallBack::GetTool() const
{
	return Tool;
}

Editor::Editor()
{
	ToolBox = std::make_unique<EditToolBox>();
	ToolBox->SetupTools( this );

	MouseLButtonTool = ToolBox->GetMouseButtonTool( EditTool_MouseButton_None::TypeName );
	MouseRButtonTool = ToolBox->GetMouseButtonTool( EditTool_MouseButton_Delete::TypeName );

	_Map = std::make_unique<Map>( "" );
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
		SetControlKeys( c, true );
		switch ( c )
		{
		case VK_BACK:
		case VK_DELETE:
			_Map->GetGrid().DeleteSelectedCells();
			ke.SetHandled( true );
			break;
		case VK_ESCAPE:
			_Map->GetGrid().ClearSelectedCells();
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
	else if ( ke.IsChar() )
	{
		switch ( c )
		{
		case '`':
		case '~':
			_Map->GetGrid().ToggleGridDrawing();
			ke.SetHandled( true );
			break;
		default:
			break;
		}
	}
	else if ( ke.IsRelease() )
	{
		SetControlKeys( c, false );
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
		MouseLPress( me );
		break;
	case Mouse::Event::Type::LRelease:
		MouseLRelease( me );
		break;
	case Mouse::Event::Type::Move:
		MouseMove( me );
		break;
	case Mouse::Event::Type::MPress:
		MouseMPress( me );
		break;
	case Mouse::Event::Type::RPress:
		MouseRPress( me );
		break;
	case Mouse::Event::Type::WheelUp:
	case Mouse::Event::Type::WheelDown:
		MouseWheel( me );
		break;
	default:
		break;
	}
}

void Editor::Draw( Graphics& gfx )
{
	_Map->GetGrid().Draw( gfx );

	// Highlight currently-hovered cell
	if ( _Map->GetGrid().IsOnGrid( MouseInf.HoverGridLocation ) )
	{
		if ( typeid(*MouseLButtonTool) == typeid(EditTool_MouseButton_Insert) )
		{
			_Map->GetGrid().DrawCell( MouseInf.HoverGridLocation, ((EditTool_MouseButton_Insert*)MouseLButtonTool)->GetFixture(), gfx );
		}

		_Map->GetGrid().HighlightCell( MouseInf.HoverGridLocation, MouseLButtonTool->GetToolColour(), EditConstants::CellEditing::CellHoverOpacity, true, gfx );
	}
}

const bool Editor::GetControlModeEnabled() const
{
	return ControlModeEnabled;
}

Map& Editor::GetMap()
{
	return *_Map;
}

Editor::MouseInfo& Editor::GetMouseInfo()
{
	return MouseInf;
}

RectI Editor::GetSelectionRectangle( const Vei2& gridLocation ) const
{
	Vei2 startLocation = MouseInf.LMouseButtonGridLocationAtLPress;
	Vei2 topLeft( std::min( startLocation.x, gridLocation.x ), std::min( startLocation.y, gridLocation.y ) );
	Vei2 bottomRight( std::max( startLocation.x, gridLocation.x ), std::max( startLocation.y, gridLocation.y ) );

	return RectI( topLeft, bottomRight );
}

const bool Editor::GetShiftModeEnabled() const
{
	return ShiftModeEnabled;
}

const EditToolBox & Editor::GetToolBox() const
{
	return *ToolBox;
}

void Editor::SetMap( std::unique_ptr<Map> map )
{
	_Map = std::move( map );
}

void Editor::CycleMouseLClickMode()
{
	if ( GetShiftModeEnabled() )
	{
		// Don't cycle through while we're in forced selection mode, or the user could get confused.
		return;
	}

	if ( MouseLButtonToolSelectionOverrideParked != nullptr )
	{
		DisableSelectionMode();
		return;
	}

	MouseLClickMode = (EditConstants::MouseLClickMode)(((int)MouseLClickMode + 1) % (int)EditConstants::MouseLClickMode::EnumOptionsCount);

	switch ( MouseLClickMode )
	{
	case EditConstants::MouseLClickMode::None:
		MouseLButtonTool = ToolBox->GetMouseButtonTool( EditTool_MouseButton_None::TypeName );
		break;
	case EditConstants::MouseLClickMode::Insert:
		MouseLButtonTool = ToolBox->GetMouseButtonTool( EditTool_MouseButton_Insert::TypeName );
		break;
	case EditConstants::MouseLClickMode::Delete:
		MouseLButtonTool = ToolBox->GetMouseButtonTool( EditTool_MouseButton_Delete::TypeName );
		break;
	case EditConstants::MouseLClickMode::Select:
		MouseLButtonTool = ToolBox->GetMouseButtonTool( EditTool_MouseButton_Select::TypeName );
		break;
	case EditConstants::MouseLClickMode::Move:
		MouseLButtonTool = ToolBox->GetMouseButtonTool( EditTool_MouseButton_Move::TypeName );
		break;
	default:
		break;
	}
}

void Editor::DisableSelectionMode()
{
	if ( MouseLButtonToolSelectionOverrideParked != nullptr )
	{
		MouseLButtonTool = MouseLButtonToolSelectionOverrideParked;
		MouseLButtonToolSelectionOverrideParked = nullptr;
	}
}

void Editor::EnableSelectionMode()
{
	if ( _Map->GetGrid().IsOnGrid( MouseInf.LMouseButtonGridLocationAtLPress ) )
	{
		// Already have left mouse button down, only do toggling beforehand
		return;
	}

	if ( MouseLClickMode != EditConstants::MouseLClickMode::Select )
	{
		if ( MouseLButtonToolSelectionOverrideParked == nullptr )
		{
			MouseLButtonToolSelectionOverrideParked = MouseLButtonTool;
		}
		MouseLButtonTool = ToolBox->GetMouseButtonTool( EditTool_MouseButton_Select::TypeName );
	}
}

void Editor::MouseLDrag( const Vei2 & gridLocation )
{
	if ( !_Map->GetGrid().IsOnGrid( gridLocation ) )
	{
		return;
	}

	MouseLButtonTool->MouseMoved();
}

void Editor::MouseLPress( Mouse::Event& me )
{
	const Vei2 gridLocation = _Map->GetGrid().ScreenToGrid( me.GetPos() );
	if ( !_Map->GetGrid().IsOnGrid( gridLocation ) )
	{
		return;
	}

	if ( gridLocation == MouseInf.LMouseButtonGridLocation )
	{
		// Don't want to be automatically clicking the same thing more than once
		return;
	}

	MouseInf.LMouseButtonGridLocation = gridLocation;

	if ( !_Map->GetGrid().IsOnGrid( MouseInf.LMouseButtonGridLocationAtLPress ) )
	{
		// Set this location only when first pressing the left mouse button down (start press location)
		MouseInf.LMouseButtonGridLocationAtLPress = gridLocation;
	}

	MouseLButtonTool->ButtonPressed();

	me.SetHandled( true );
}

void Editor::MouseLRelease( Mouse::Event& me )
{
	MouseInf.LMouseButtonGridLocation = Vei2( -1, -1 );
	MouseInf.LMouseButtonGridLocationAtLPress = Vei2( -1, -1 );

	MouseLButtonTool->ButtonReleased();

	me.SetHandled( true );
}

void Editor::MouseMDrag( Mouse::Event& me )
{
	const Vei2 mousePos = me.GetPos();
	const Vei2 delta = mousePos - MouseInf.MMouseButtonLocation;
	MouseInf.MMouseButtonLocation = mousePos;
	_Map->GetGrid().Move( (Vec2)delta );
}

void Editor::MouseMove( Mouse::Event& me )
{
	MouseInf.HoverGridLocation = _Map->GetGrid().ScreenToGrid( me.GetPos() );

	if ( me.LeftIsPressed() )
	{
		MouseLPress( me );
		MouseLDrag( MouseInf.HoverGridLocation );
	}

	if ( me.MiddleIsPressed() )
	{
		//MouseMPress( me ); ?
		MouseMDrag( me );
	}

	if ( me.RightIsPressed() )
	{
		MouseRPress( me );
		//MouseRDrag( me ); ?
	}

	me.SetHandled( true );
}

void Editor::MouseMPress( Mouse::Event& me )
{
	MouseInf.MMouseButtonLocation = me.GetPos();
	me.SetHandled( true );
}

void Editor::MouseRPress( Mouse::Event& me )
{
	MouseRButtonTool->ButtonPressed();
	me.SetHandled( true );
}

void Editor::MouseWheel( Mouse::Event& me )
{
	_Map->GetGrid().Zoom( (Vec2)me.GetPos(), me.GetType() == Mouse::Event::Type::WheelUp );
	me.SetHandled( true );
}

void Editor::SetControlKeys( const unsigned char c, const bool enabled )
{
	switch ( c )
	{
	case VK_CONTROL:
		ControlModeEnabled = enabled;
		break;
	case VK_SHIFT:
		ShiftModeEnabled = enabled;
		break;
	default:
		break;
	}
}

void Editor::SetMouseLButtonTool( EditTool_MouseButton* const tool )
{
	MouseLButtonTool = tool;
}

void Editor::EditTool_MouseButton_InsertLCallBack::Execute() const
{
	((EditTool_MouseButton_Insert*)GetTool())->SetFixture( Fixture );
	EditTool_MouseButtonLCallBack::Execute();
}
