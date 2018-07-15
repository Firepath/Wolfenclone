#pragma once

#include "Menu.h"

void TestCallBack::Execute() const
{
	int i = 5;
}

Menu::MenuItem::MenuItem( std::string text, const Menu& menu, std::unique_ptr<SelectedCallBack> callback )
	:
	Text( text ),
	_Menu( menu ),
	CallBack( std::move( callback ) )
{
	Height = _Menu._Font.GetGlyphHeight() + 2 * _Menu.BoxPadding;
	Width = _Menu.MaximumWidth;
}

const Vei2 Menu::MenuItem::GetSize() const
{
	return Vei2( (int)Width, (int)Height );
}

const std::string Menu::MenuItem::GetText() const
{
	return Text;
}

void Menu::MenuItem::SetText( const std::string text )
{
	Text = text;
}

void Menu::MenuItem::Select()
{
	CallBack->Execute();
}

Menu::Menu( const Font& font )
	:
	_Font( font )
{
}

void Menu::AddMenuItem( std::string text, std::unique_ptr<SelectedCallBack> callback )
{
	MenuItems.emplace_back( text, *this, std::move( callback ) );
}

const Vei2 Menu::GetLocation()
{
	return Location;
}

void Menu::SetLocation( const Vei2 screenLocation )
{
	Location = screenLocation;
}

void Menu::Show( Graphics& gfx )
{
	// TODO: Smarts to make sure menu shows on screen as best it can
	// TODO: Further smarts to allow scrolling menu that is off the screen

	PixelEffect::Transparency boxEffect( Opacity );

	Vei2 location = Location;
	for ( auto it = MenuItems.begin(); it != MenuItems.end(); it++ )
	{
		it->Draw( location, boxEffect, gfx );
		location.y += it->GetSize().y + 1;
	}
}
