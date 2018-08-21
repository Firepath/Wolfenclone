#pragma once

#include "Menu.h"

void TestCallBack::Execute() const
{
	int i = 5;
}

MenuItem::MenuItem( std::string text, std::unique_ptr<SelectedCallBack> callback, const MenuItem* const menu, const Font* const font, Graphics& gfx )
	:
	Text( text ),
	CallBack( std::move( callback ) ),
	_Menu( menu ),
	_Font( font ),
	_gfx( gfx )
{
	if ( _Font != nullptr )
	{
		Height = _Font->GetHeight() + 2 * BoxPadding;
		Width = std::min( _Font->GetWidth() * Text.length() + 2 * BoxPadding, Width );
	}
}

void MenuItem::AddMenuItem( std::unique_ptr<MenuItem> menuItem )
{
	MenuItems.push_back( std::move( menuItem ) );

	bool larger = true;
	size_t maxWidth = MenuItems.back()->GetWidth();
	for ( auto it = MenuItems.begin(); it != MenuItems.end(); it++ )
	{
		const Vei2 size = it->get()->GetSize();
		if ( size.x > (int)maxWidth )
		{
			maxWidth = size.x;
			larger = false;
			break;
		}
	}

	if ( larger )
	{
		for ( auto it = MenuItems.begin(); it != MenuItems.end(); it++ )
		{
			it->get()->SetWidth( maxWidth );
		}
	}
	else
	{
		MenuItems.back()->SetWidth( maxWidth );
	}
}

void MenuItem::AddMenuItem( std::string text, std::unique_ptr<SelectedCallBack> callback )
{
	std::unique_ptr<MenuItem> item = std::make_unique<MenuItem>( text, std::move( callback ), this, _Font, _gfx );
	AddMenuItem( std::move( item ) );
}

void MenuItem::DoHovering( const bool hovering, const bool hoveringOnChild )
{
	if ( !IsOpen() && hovering && MenuItems.size() > 0 && typeid(*this) == typeid(MenuItem) )
	{
		SetOpen( true );
	}

	TextColour = MenuItem::DefaultTextColour;
	if ( hovering || hoveringOnChild )
	{
		TextColour = MenuItem::DefaultHoverTextColour;
	}
}

void MenuItem::DoKeyboardEvents( Keyboard::Event& ke )
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
		default:
			break;
		}
	}
	else if ( ke.IsRelease() )
	{
		switch ( c )
		{
		default:
			break;
		}
	}
}

void MenuItem::DoMouseEvents( Mouse::Event& me )
{
	const Vei2 mousePos = me.GetPos();
	const bool mouseIsOnMenuItem = IsHovering( mousePos, true );

	for ( auto it = MenuItems.begin(); it != MenuItems.end(); it++ )
	{
		MenuItem& item = *(it->get());

		const bool isHovering = item.IsVisible() && item.IsHovering( mousePos, false );

		if ( !isHovering )
		{
			item.TextColour = MenuItem::DefaultTextColour;

			if ( &item != this && item.IsOpen() )
			{
				item.SetOpen( false );
			}
		}
	}

	DoHovering( mouseIsOnMenuItem, IsHovering( mousePos, false ) );

	// Something else has already handled this
	if ( me.IsHandled() )
	{
		return;
	}

	if ( mouseIsOnMenuItem )
	{
		Mouse::Event::Type meType = me.GetType();

		switch ( meType )
		{
		case Mouse::Event::Type::LRelease:
			Select();
			break;
		default:
			break;
		}

		me.SetHandled( true );
	}
	else
	{
		if ( IsOpen() )
		{
			for ( auto it = MenuItems.begin(); it != MenuItems.end() && !me.IsHandled(); it++ )
			{
				MenuItem& item = *(it->get());

				item.DoMouseEvents( me );

				// If it got handled and that menu item is no longer visible (because it got selected)
				if ( me.IsHandled() && !item.IsVisible() )
				{
					// Close the menu
					SetVisible( false );
				}
			}
		}
	}
}

const Vei2 MenuItem::GetLocation() const
{
	return Location;
}

const Vei2 MenuItem::GetSize() const
{
	return Vei2( (int)Width, (int)Height );
}

const std::string MenuItem::GetText() const
{
	return Text;
}

const size_t MenuItem::GetWidth() const
{
	return Width;
}

const bool MenuItem::IsOpen() const
{
	return MenuItems.size() > 0 && Open;
}

const bool MenuItem::IsVisible() const
{
	return Visible;
}

void MenuItem::Select()
{
	if ( CallBack != nullptr )
	{
		CallBack->Execute();
		SetVisible( false );
	}
	else if ( MenuItems.size() > 0 )
	{
		if ( !IsOpen() )
		{
			ShowMenu();
		}
	}
}

void MenuItem::SetLocation( const Vei2 screenLocation )
{
	Location = screenLocation;
}

void MenuItem::SetOpen( const bool open )
{
	if ( open == Open )
	{
		// Nothing to do
		return;
	}

	Open = open;

	if ( !IsOpen() )
	{
		for ( auto it = MenuItems.begin(); it != MenuItems.end(); it++ )
		{
			MenuItem& item = *(it->get());
			item.SetVisible( false );
			if ( item.IsOpen() )
			{
				item.SetOpen( false );
			}
		}
	}
}

void MenuItem::SetText( const std::string text )
{
	Text = text;
}

void MenuItem::SetVisible( const bool visible )
{
	Visible = visible;

	if ( !IsVisible() )
	{
		SetOpen( false );
	}
}

void MenuItem::SetWidth( const size_t width )
{
	Width = width;
}

void MenuItem::ShowMenu()
{
	const Vei2 topLeft = Location + Vei2( (int)Width, 0 );
	ShowMenu( topLeft );
}

const bool MenuItem::IsHovering( const Vei2 mouseLocation, const bool onlyCheckThisMenuItem ) const
{
	assert( IsVisible() );

	bool hovering = RectI( Location, GetSize().x, GetSize().y ).Contains( mouseLocation );

	if ( !onlyCheckThisMenuItem && !hovering )
	{
		for ( auto it = MenuItems.begin(); it != MenuItems.end(); it++ )
		{
			MenuItem& item = *(it->get());

			// but we are hovering on a child item,
			if ( item.IsVisible() && item.IsHovering( mouseLocation, false ) )
			{
				// then consider us as hovering on this item.
				return true;
			}
		}
	}

	return hovering;
}

void MenuItem::ShowMenu( const Vei2 location )
{
	assert( MenuItems.size() > 0 );

	SetOpen( true );

	PixelEffect::Transparency boxEffect( Opacity );

	size_t itemsWidth = Width;
	Vei2 itemLocation = location;
	for ( auto it = MenuItems.begin(); it != MenuItems.end(); it++ )
	{
		it->get()->Show( itemLocation, boxEffect );
		const Vei2 itemSize = it->get()->GetSize();
		itemLocation.y += itemSize.y;
		itemsWidth = itemSize.x;
	}

	const RectI border = RectI( location, Vei2( itemLocation.x + (int)itemsWidth, itemLocation.y ) );
	const RectI insideBox = border.GetExpanded( -(int)BorderThickness );
	_gfx.DrawBoxBorder( border, insideBox, BorderColour, boxEffect );
}

Menu::Menu( std::string text, const Font* const font, Graphics& gfx )
	:
	MenuItem(text, nullptr, this, font, gfx )
{
}

void Menu::ShowMenu()
{
	const Vei2 topLeft = Location + Vei2( 0, (int)Height - 2 );
	MenuItem::ShowMenu( topLeft );
}

MenuBar::MenuBar( const Vei2 location, const Vei2 size, Graphics& gfx )
	:
	Location( location ),
	_gfx( gfx ),
	Size( size )
{
}

void MenuBar::AddMenu( std::unique_ptr<Menu> menu )
{
	const int menuHeight = menu->GetSize().y;
	if ( menuHeight > Size.y )
	{
		Size.y = menuHeight - 1;
	}

	Menus.push_back( std::move( menu ) );
}

void MenuBar::DoKeyboardEvents( Keyboard::Event & ke )
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
		case VK_ESCAPE:
			CancelMenus();
			break;
		default:
			break;
		}
	}
	else if ( ke.IsRelease() )
	{
		switch ( c )
		{
		default:
			break;
		}
	}

	for ( auto it = Menus.begin(); it != Menus.end() && !ke.IsHandled(); it++ )
	{
		it->get()->DoKeyboardEvents( ke );
	}
}

void MenuBar::DoMouseEvents( Mouse::Event& me )
{
	// Something else has already handled this
	if ( me.IsHandled() )
	{
		return;
	}

	const Vei2 mousePos = me.GetPos();

	for ( auto it = Menus.begin(); it != Menus.end() && !me.IsHandled(); it++ )
	{
		if ( it->get()->IsVisible() )
		{
			it->get()->DoMouseEvents( me );
		}
	}

	// If none of the menus handled this event,
	if ( !me.IsHandled() )
	{
		Mouse::Event::Type meType = me.GetType();
		switch ( meType )
		{
			// And it was a click of some sort,
		case Mouse::Event::Type::LPress:
		case Mouse::Event::Type::MPress:
		case Mouse::Event::Type::RPress:
			for ( auto it = Menus.begin(); it != Menus.end(); it++ )
			{
				// Hide all the open menus
				it->get()->SetOpen( false );
			}
			break;
		default:
			break;
		}
	}
}

void MenuBar::Draw() const
{
	PixelEffect::Transparency boxEffect( Opacity );
	const RectI border = RectI( Location, Size.x, Size.y );
	const RectI insideBox = border.GetExpanded( -(int)BorderThickness );
	_gfx.DrawBox( insideBox, BoxColour, boxEffect );

	Vei2 location = Location;
	for (auto it = Menus.begin(); it != Menus.end(); it++)
	{
		Menu* menu = it->get();
		menu->Show( location, PixelEffect::Copy() );
		location.y += (int)menu->GetWidth();
	}

	_gfx.DrawBoxBorder( border, insideBox, BorderColour, boxEffect );
}

void MenuBar::CancelMenus()
{
	for ( auto it = Menus.begin(); it != Menus.end(); it++ )
	{
		if ( it->get()->IsOpen() )
		{
			it->get()->SetOpen( false );
		}
	}
}
