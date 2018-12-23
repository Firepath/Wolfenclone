#pragma once

#include "EditConstants.h"
#include "Editor.h"
#include "Graphics.h"
#include "MapSaver.h"
#include "Menu.h"
#include "PixelEffect.h"

MenuItem* MenuItem::Menu_ItemSelectedCallBack::GetMenuItem() const
{
	return _MenuItem;
}

void MenuItem::Menu_FileSaveItemSelectedCallBack::Execute() const
{
	MapSaver saver( _Editor->GetMap() );
	saver.Save( "Maps\\Map1.txt" ); // TODO: Use map name from editor
}

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
	StackMenuItem( std::move( menuItem ), MenuItems.size() );
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
	//if ( ke.IsPress() )
	//{
	//	switch ( c )
	//	{
	//	default:
	//		break;
	//	}
	//}
	//else if ( ke.IsRelease() )
	//{
	//	switch ( c )
	//	{
	//	default:
	//		break;
	//	}
	//}
}

void MenuItem::DoMouseEvents( Mouse::Event& me )
{
	const Vei2 mousePos = me.GetPos();
	const bool mouseIsOnMenuItem = IsHovering( mousePos, true );

	DoHovering( IsHovering( mousePos, false ) );
	ResetSubMenuItems( mousePos );

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

void MenuItem::StackMenuItem( std::unique_ptr<MenuItem> menuItem, const size_t position )
{
	MenuItem* const currentMenuItem = menuItem.get();

	MenuItems.insert( MenuItems.begin() + position, std::move( menuItem ) );

	bool larger = false;
	size_t maxWidth = currentMenuItem->GetWidth();
	for ( auto it = MenuItems.begin(); it != MenuItems.end(); it++ )
	{
		const int width = it->get()->GetSize().x;
		if ( width > (int)maxWidth )
		{
			maxWidth = width;
			larger = true;
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
		// All others are set as larger than this one, set this one too
		currentMenuItem->SetWidth( maxWidth );
	}
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
		if ( !IsOpen() && MenuItems.size() > 0 )
		{
			SetOpen( true );
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
		SetHovering( false );
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

void MenuItem::DoHovering( const bool hovering )
{
	SetHovering( hovering );

	if ( !IsOpen() &&
		MenuItems.size() > 0 &&
		hovering &&
		(typeid(*this) == typeid(MenuItem) ) )
	{
		SetOpen( true );
	}

	TextColour = MenuItem::DefaultTextColour;
	if ( IsHovering() )
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

const bool MenuItem::IsHovering()
{
	return Hovering;
}

const bool MenuItem::IsHovering( const Vei2 mouseLocation, const bool onlyCheckThisMenuItem )
{
	if ( !IsVisible() )
	{
		return false;
	}

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

			if ( item.IsOpen() )
			{
				item.SetOpen( false );
			}
		}
	}
}

void MenuItem::SetHovering( bool hovering )
{
	Hovering = hovering;
}

void MenuItem::ShowMenu( const Vei2 location )
{
	assert( MenuItems.size() > 0 );
	assert( IsOpen() );

	const RectI subMenuBorder = GetSubMenuArea();
	const RectI subMenuInnerBorder = subMenuBorder.GetExpanded( -(int)BorderThickness );

	std::unique_ptr<PixelEffect::Effect> boxEffect = std::make_unique<PixelEffect::Transparency>( Opacity );
	_gfx.DrawBox( subMenuInnerBorder, BoxColour, boxEffect );
	_gfx.DrawBoxBorder( subMenuBorder, subMenuInnerBorder, BorderColour, boxEffect );
	
	ShowSubMenu( location, boxEffect, false );
	ShowSubMenu( location, boxEffect, true );
}

void MenuItem::ShowSubMenu( const Vei2 location, std::unique_ptr<PixelEffect::Effect>& boxEffect, bool onlyHovering )
{
	size_t column = 0;
	size_t currentRowHeight = 0;
	Vei2 itemLocation = location;
	int column0X = itemLocation.x;
	for ( auto it = MenuItems.begin(); it != MenuItems.end(); it++ )
	{
		MenuItem& item = *(it->get());

		if ( onlyHovering == item.IsHovering() )
		{
			item.Show( itemLocation, boxEffect );
			if ( onlyHovering )
			{
				if ( item.IsOpen() )
				{
					item.ShowMenu();
				}
				break; // Should only be one...
			}
		}

		const Vei2 itemSize = item.GetSize();
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

	DoHovering( IsHovering( mousePos, false ) );
	ResetSubMenuItems( mousePos );

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
	//else if ( ke.IsRelease() )
	//{
	//	switch ( c )
	//	{
	//	default:
	//		break;
	//	}
	//}

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

	for ( auto it = Menus.begin(); it != Menus.end(); it++ )
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
			CancelMenus();
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
	_gfx.DrawBoxBorder( border, insideBox, BorderColour, boxEffect );

	Vei2 location = Location;
	for (auto it = Menus.begin(); it != Menus.end(); it++)
	{
		MenuItem* menu = it->get();
		std::unique_ptr<PixelEffect::Effect> copy = std::make_unique<PixelEffect::Copy>();
		menu->Show( location, copy );
		location.x += (int)menu->GetWidth();
	}

	// Similarly should be doing this but reset or something
	// is breaking it
	for ( auto it = Menus.begin(); it != Menus.end(); it++ )
	{
		MenuItem* menu = it->get();
		if ( menu->IsOpen() )
		{
			menu->ShowMenu();
			break; // Should only be one...
		}
	}
}

void MenuBar::CancelMenus()
{
	for ( auto it = Menus.begin(); it != Menus.end(); it++ )
	{
		if ( it->get()->IsOpen() )
		{
			// Hide all the open menus
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

	const RectI boxRect = RectI( location + Vei2( 1, 1 ), (int)Width - 1, (int)Height - 1 );
	_gfx.DrawBox( boxRect, BoxColour, effect );

	if ( Image != nullptr )
	{
		RectI imageRect = boxRect.GetExpanded( -(int)BoxPadding );

		std::unique_ptr<PixelEffect::Effect> effect = std::make_unique<PixelEffect::Chroma>();

		if ( IsHovering() )
		{
			const RectI highlightRect = boxRect.GetExpanded( std::max( (int)BoxPadding, boxRect.GetWidth() / 4 ) );
			imageRect = highlightRect.GetExpanded( -(int)BoxPadding );
			_gfx.DrawBoxBorder( highlightRect, imageRect, HighlightColour, effect );
		}

		_gfx.DrawSprite( imageRect, *Image, effect );
	}
}
