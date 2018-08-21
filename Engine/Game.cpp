/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

#include "PixelEffect.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	_Editor = std::make_unique<Editor>();

	Surfaces = std::make_unique<SurfaceRepository>();
	Fonts = std::make_unique<FontRepository>();

	LoadSettings();
	SetupMenu();
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	DoKeyboardEvents();
	DoMouseEvents();
}

void Game::DoKeyboardEvents()
{
	while ( !wnd.kbd.KeyIsEmpty() )
	{
		Keyboard::Event e = wnd.kbd.ReadKey();
		MainMenuBar->DoKeyboardEvents( e );
		_Editor->DoKeyboardEvents( e );
	}
}

void Game::DoMouseEvents()
{
	while ( !wnd.mouse.IsEmpty() )
	{
		Mouse::Event e = wnd.mouse.Read();
		MainMenuBar->DoMouseEvents( e );
		_Editor->DoMouseEvents( e );
	}
}

void Game::LoadSettings()
{
	Surfaces->AddSurface( "Font_Fixedsys16x28", std::make_unique<Surface>( "Textures\\Fonts\\Fixedsys16x28.bmp" ) );

	Fonts->AddFont( "Font_Fixedsys16x28", std::make_unique<Font>( Surfaces->GetSurface( "Font_Fixedsys16x28" ), Colors::White, 28, Colors::White ) );
}

void Game::SetupMenu()
{
	Font* menuFont = &Fonts->GetFont( "Font_Fixedsys16x28" );

	//MainMenuBar = std::make_unique<MenuBar>( Vei2( 0, 0 ), Vei2( Graphics::ScreenWidth, menuFont->GetHeight() + (int)Menu::DefaultBoxPadding * 2 ), gfx );
	MainMenuBar = std::make_unique<MenuBar>( Vei2( 0, 0 ), Vei2( Graphics::ScreenWidth, 10 ), gfx );

	//MainMenu = std::make_unique<Menu>( "Main Menu", MainMenuFont.get(), gfx );
	std::unique_ptr<Menu> MainMenu = std::make_unique<Menu>( "Main Menu", menuFont, gfx );
	//MainMenu->SetLocation( { 20,20 } );
	//MainMenu->AddMenuItem( "!\"#$%&'()*+,-./0123456789:;<=>?", std::make_unique<TestCallBack>() );
	//MainMenu->AddMenuItem( "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_", std::make_unique<TestCallBack>() );
	//MainMenu->AddMenuItem( "`abcdefghijklmnopqrstuvwxyz{|}~", std::make_unique<TestCallBack>() );
	//MainMenu->AddMenuItem( "A-a_B.b,1[2]3(4){5}<6>7=8?9:0;C.c,D^d\\", std::make_unique<TestCallBack>() );
	//MainMenu->AddMenuItem( "E`e~F!f\'G#g$H%h&I'i*J+j|K", std::make_unique<TestCallBack>() );
	
	// "Real" tests



	std::unique_ptr<MenuItem> subMenu = std::make_unique<MenuItem>( "Sub-Menu", nullptr, MainMenu.get(), menuFont, gfx );

	std::unique_ptr<MenuItem> subSubMenu1 = std::make_unique<MenuItem>( "Sub Option 1", std::make_unique<TestCallBack>(), subMenu.get(), menuFont, gfx );
	subSubMenu1->AddMenuItem( "Sub-Sub 1 Option 1", std::make_unique<TestCallBack>() );
	subSubMenu1->AddMenuItem( "Sub-Sub 1 Option 2", std::make_unique<TestCallBack>() );

	std::unique_ptr<MenuItem> subSubMenu3 = std::make_unique<MenuItem>( "Sub Option 3", std::make_unique<TestCallBack>(), subMenu.get(), menuFont, gfx );
	subSubMenu3->AddMenuItem( "Sub-Sub 3 Option 1", std::make_unique<TestCallBack>() );
	subSubMenu3->AddMenuItem( "Sub-Sub 3 Option 2", std::make_unique<TestCallBack>() );

	//subMenu->AddMenuItem( "Sub Option 1", std::make_unique<TestCallBack>() );
	subMenu->AddMenuItem( std::move( subSubMenu1 ) );
	subMenu->AddMenuItem( "Sub Option 2", std::make_unique<TestCallBack>() );
	//subMenu->AddMenuItem( "Sub Option 3", std::make_unique<TestCallBack>() );
	subMenu->AddMenuItem( std::move( subSubMenu3 ) );
	subMenu->AddMenuItem( "Sub Option 4", std::make_unique<TestCallBack>() );

	MainMenu->AddMenuItem( "Place", std::make_unique<TestCallBack>() );
	MainMenu->AddMenuItem( "Select", std::make_unique<TestCallBack>() );
	MainMenu->AddMenuItem( "Really long option", std::make_unique<TestCallBack>() );
	MainMenu->AddMenuItem( std::move( subMenu ) );
	MainMenu->AddMenuItem( "Go", std::make_unique<TestCallBack>() );

	MainMenuBar->AddMenu( std::move( MainMenu ) );
}

void Game::ComposeFrame()
{
	_Editor->Draw( gfx );
	MainMenuBar->Draw();
}
