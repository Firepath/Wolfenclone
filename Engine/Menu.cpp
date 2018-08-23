#pragma once

#include "EditConstants.h"
#include "Graphics.h"
#include "Menu.h"
#include "PixelEffect.h"

MenuItem::MenuItem( std::string text, std::unique_ptr<SelectedCallBack> callback, const MenuItem* const menu, const Font* const font, Graphics& gfx, const Color highlightColour )
	:
	Text( text ),
	CallBack( std::move( callback ) ),
	_Menu( menu ),
	_Font( font ),
	_gfx( gfx ),
	HighlightColour( highlightColour )
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

void MenuItem::AddMenuItem( std::string text, std::unique_ptr<SelectedCallBack> callback, Color highlightColour )
{
	std::unique_ptr<MenuItem> item = std::make_unique<MenuItem>( text, std::move( callback ), this, _Font, _gfx, highlightColour );
	AddMenuItem( std::move( item ) );
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

	ResetSubMenuItems( mousePos );

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
		DoSubMenuMouseEvents( me );
	}
}

const size_t MenuItem::GetColumns() const
{
	return Columns;
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

void MenuItem::SetColumns( const size_t columns )
{
	Columns = columns;
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

void MenuItem::DoHovering( const bool hovering, const bool hoveringOnChild )
{
	if ( !IsOpen() && hovering && MenuItems.size() > 0 && typeid(*this) == typeid(MenuItem) )
	{
		SetOpen( true );
	}

	TextColour = MenuItem::DefaultTextColour;
	if ( hovering || hoveringOnChild )
	{
		TextColour = HighlightColour;
	}
}

void MenuItem::DoSubMenuMouseEvents( Mouse::Event& me )
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

const RectI MenuItem::GetSubMenuArea() const
{
	const Vei2 subMenuLocation = GetSubMenuLocation();
	return RectI( subMenuLocation, subMenuLocation + GetSubMenuSize() );
}

const Vei2 MenuItem::GetSubMenuLocation() const
{
	const Vei2 size = GetSize();
	Vei2 subMenuLocation( Location.x + size.x, Location.y );
	return subMenuLocation;
}

const Vei2 MenuItem::GetSubMenuSize() const
{
	size_t itemsWidth = 0;
	size_t currentRowWidth = 0;
	size_t itemsHeight = 0;
	size_t currentRowHeight = 0;

	size_t column = 0;
	for ( auto it = MenuItems.begin(); it != MenuItems.end(); it++ )
	{
		const Vei2 itemSize = it->get()->GetSize();
		currentRowWidth += itemSize.x;
		currentRowHeight = std::max( (int)currentRowHeight, itemSize.y );

		column++;
		column = column % Columns;

		if ( column == 0 )
		{
			itemsWidth = std::max( itemsWidth, currentRowWidth );
			currentRowWidth = 0;
			itemsHeight += currentRowHeight;
			currentRowHeight = 0;
		}
	}

	if ( column != 0 )
	{
		itemsHeight += currentRowHeight;
		currentRowHeight = 0;
		column = 0;
	}

	if ( itemsWidth == 0 )
	{
		itemsWidth += currentRowWidth;
	}

	return Vei2( itemsWidth, itemsHeight );
}

const bool MenuItem::IsHovering( const Vei2 mouseLocation, const bool onlyCheckThisMenuItem )
{
	assert( IsVisible() );

	Hovering = RectI( Location, GetSize().x, GetSize().y ).Contains( mouseLocation );

	if ( !onlyCheckThisMenuItem && !Hovering && IsOpen() )
	{
		const RectI subMenuArea = GetSubMenuArea();
		if ( subMenuArea.Contains( mouseLocation ) )
		{
			return true;
		}
		else
		{
			for ( auto it = MenuItems.begin(); it != MenuItems.end(); it++ )
			{
				MenuItem& item = *(it->get());

				if ( item.IsOpen() && item.IsHovering( mouseLocation, false ) )
				{
					return true;
				}
			}
		}
	}

	return Hovering;
}

void MenuItem::ResetSubMenuItems( const Vei2 mousePos )
{
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
}

void MenuItem::ShowMenu( const Vei2 location )
{
	assert( MenuItems.size() > 0 );

	SetOpen( true );

	const Vei2 subMenuSize = GetSubMenuSize();

	const RectI border = RectI( location, Vei2( location.x + subMenuSize.x, location.y + subMenuSize.y ) );
	const RectI insideBox = border.GetExpanded( -(int)BorderThickness );

	std::unique_ptr<PixelEffect::Effect> boxEffect = std::make_unique<PixelEffect::Transparency>( Opacity );
	_gfx.DrawBox( insideBox, BoxColour, boxEffect );

	size_t column = 0;
	size_t currentRowHeight = 0;
	Vei2 itemLocation = location;
	int column0X = itemLocation.x;
	for ( auto it = MenuItems.begin(); it != MenuItems.end(); it++ )
	{
		it->get()->Show( itemLocation, boxEffect );

		const Vei2 itemSize = it->get()->GetSize();
		currentRowHeight = std::max( (int)currentRowHeight, itemSize.y );

		itemLocation.x += itemSize.x;

		column++;
		column = column % Columns;

		if ( column == 0 )
		{
			itemLocation.y += currentRowHeight;
			currentRowHeight = 0;
			itemLocation.x = column0X;
		}
	}

	_gfx.DrawBoxBorder( border, insideBox, BorderColour, boxEffect );
}

Menu::Menu( std::string text, const Font* const font, Graphics& gfx, const Color highlightColour )
	:
	MenuItem(text, nullptr, this, font, gfx, highlightColour )
{
}

void Menu::DoMouseEvents( Mouse::Event & me )
{
	const Vei2 mousePos = me.GetPos();
	const bool mouseIsOnMenuItem = IsHovering( mousePos, true );

	ResetSubMenuItems( mousePos );

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
			if ( IsOpen() )
			{
				SetOpen( false );
			}
			else
			{
				Select();
			}
			break;
		default:
			break;
		}

		me.SetHandled( true );
	}
	else
	{
		DoSubMenuMouseEvents( me );
	}
}

void Menu::ShowMenu()
{
	const Vei2 topLeft = GetSubMenuLocation() - Vei2( 0, 2 );
	MenuItem::ShowMenu( topLeft );
}

const Vei2 Menu::GetSubMenuLocation() const
{
	return Vei2( Location.x, Location.y + GetSize().y );
}

MenuBar::MenuBar( const Vei2 location, const Vei2 size, Graphics& gfx )
	:
	Location( location ),
	_gfx( gfx ),
	Size( size )
{
}

void MenuBar::AddMenu( std::unique_ptr<MenuItem> menu )
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
	std::unique_ptr<PixelEffect::Effect> boxEffect = std::make_unique<PixelEffect::Transparency>( Opacity );
	const RectI border = RectI( Location, Size.x, Size.y );
	const RectI insideBox = border.GetExpanded( -(int)BorderThickness );
	_gfx.DrawBox( insideBox, BoxColour, boxEffect );

	Vei2 location = Location;
	for (auto it = Menus.begin(); it != Menus.end(); it++)
	{
		MenuItem* menu = it->get();
		std::unique_ptr<PixelEffect::Effect> copy = std::make_unique<PixelEffect::Copy>();
		menu->Show( location, copy );
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

ImageMenuItem::ImageMenuItem( const Surface* const image, const int height, const int width, std::unique_ptr<SelectedCallBack> callback, const MenuItem* const menu, Graphics& gfx, const Color highlightColour )
	:
	MenuItem( "", std::move( callback ), menu, nullptr, gfx, highlightColour ),
	Image( image )
{
	Height = height + 2 * BoxPadding;
	Width = width + 2 * BoxPadding;
}

void ImageMenuItem::Show( const Vei2 & location, std::unique_ptr<PixelEffect::Effect>& effect )
{
	Location = location;
	Visible = true;

	const RectI boxRect = RectI( location, (int)Width, (int)Height );
	const RectI imageRect = boxRect.GetExpanded( -(int)BoxPadding );
	_gfx.DrawBox( boxRect, BoxColour, effect );

	if ( Image != nullptr )
	{
		if ( Hovering )
		{
			std::unique_ptr<PixelEffect::Effect> copy = std::make_unique<PixelEffect::Copy>();
			_gfx.DrawBoxBorder( boxRect, imageRect, HighlightColour, copy );
		}

		std::unique_ptr<PixelEffect::Effect> copy = std::make_unique<PixelEffect::Copy>();
		_gfx.DrawSprite( imageRect, *Image, copy );
	}

	if ( IsOpen() )
	{
		ShowMenu();
	}
}
