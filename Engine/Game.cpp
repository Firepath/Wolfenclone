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

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	Surfaces = std::make_unique<SurfaceRepository>();

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
		const Keyboard::Event e = wnd.kbd.ReadKey();
		Editor.DoKeyboardEvents( e );
	}
}

void Game::DoMouseEvents()
{
	while ( !wnd.mouse.IsEmpty() )
	{
		const Mouse::Event e = wnd.mouse.Read();
		Editor.DoMouseEvents( e );
	}
}

void Game::LoadSettings()
{
	Surfaces->AddSurface( "Font_Fixedsys16x28", std::make_unique<Surface>( "Textures\\Fonts\\Fixedsys16x28.bmp" ) );
}

void Game::SetupMenu()
{
	MainMenuFont = std::make_unique<Font>( Surfaces->GetSurface( "Font_Fixedsys16x28" ), Colors::White, 14, Colors::White );
	MainMenu = std::make_unique<Menu>( *(MainMenuFont.get()) );
	MainMenu->SetLocation( { 20,20 } );
	MainMenu->AddMenuItem( "!\"#$%&'()*+,-./0123456789:;<=>?", std::make_unique<TestCallBack>() );
	MainMenu->AddMenuItem( "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_", std::make_unique<TestCallBack>() );
	MainMenu->AddMenuItem( "`abcdefghijklmnopqrstuvwxyz{|}~", std::make_unique<TestCallBack>() );
	MainMenu->AddMenuItem( "A-a_B.b,1[2]3(4){5}<6>7=8?9:0;C.c,D^d\\", std::make_unique<TestCallBack>() );
	MainMenu->AddMenuItem( "E`e~F!f\'G#g$H%h&I'i*J+j|K", std::make_unique<TestCallBack>() );
}

void Game::ComposeFrame()
{
	Editor.Draw( gfx );
	MainMenu->Show( gfx );
}
