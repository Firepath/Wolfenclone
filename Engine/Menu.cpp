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

void MenuItem::AddMenuItem( std::string text, std::unique_ptr<SelectedCallBack> callback )
{
	MenuItems.emplace_back( text, std::move( callback ), this, _Font, _gfx );

	bool larger = true;
	size_t maxWidth = MenuItems.back().GetWidth();
	for ( auto it = MenuItems.begin(); it != MenuItems.end(); it++ )
	{
		const Vei2 size = it->GetSize();
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
			it->SetWidth( maxWidth );
		}
	}
	else
	{
		MenuItems.back().SetWidth( maxWidth );
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

const bool MenuItem::GetVisible() const
{
	return Visible;
}

const size_t MenuItem::GetWidth() const
{
	return Width;
}

void MenuItem::Select()
{
	if ( CallBack != nullptr )
	{
		CallBack->Execute();
	}
	else if ( MenuItems.size() > 0 )
	{
		ShowMenu();
	}
}

void MenuItem::SetLocation( const Vei2 screenLocation )
{
	Location = screenLocation;
}

void MenuItem::SetText( const std::string text )
{
	Text = text;
}

void MenuItem::SetVisible( const bool visible )
{
	Visible = visible;
}

void MenuItem::SetWidth( const size_t width )
{
	Width = width;
}

void MenuItem::ShowMenu()
{
	const Vei2 topLeft = Location + Vei2( Width + 1, 0 );
	ShowMenu( topLeft );
}

void MenuItem::ShowMenu( const Vei2 location )
{
	PixelEffect::Transparency boxEffect( Opacity );

	size_t itemsWidth = Width;
	Vei2 itemLocation = location;
	for ( auto it = MenuItems.begin(); it != MenuItems.end(); it++ )
	{
		it->Show( itemLocation, boxEffect );
		const Vei2 itemSize = it->GetSize();
		itemLocation.y += itemSize.y + 1;
		itemsWidth = itemSize.x;
	}

	const RectI border = RectI( location, Vei2( itemLocation.x + itemsWidth, itemLocation.y - 1 ) );
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
	const Vei2 topLeft = Location + Vei2( 0, Height + 1 );
	MenuItem::ShowMenu( topLeft );
}

MenuBar::MenuBar( const Vei2 location )
	:
	Location( location )
{
}

void MenuBar::AddMenu( const std::string name, std::unique_ptr<Menu> menu )
{
	Menus.push_back( std::move( menu ) );
}

//const Vei2 MenuBar::GetLocation() const
//{
//	return Location;
//}
//
//void MenuBar::SetLocation( const Vei2 screenLocation )
//{
//	Location = screenLocation;
//}
