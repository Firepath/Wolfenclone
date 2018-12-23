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

#include "FileIOConstants.h"
#include "FileListLoader.h"
#include "FixtureLoader.h"
#include "FontLoader.h"
#include "MapLoader.h"
#include "MenuLoader.h"
#include "TextureLoader.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	_Editor = std::make_unique<Editor>();
	FileListLoader fileListLoader;
	fileListLoader.Load( "Settings\\Settings.txt" );

	TextureLoader textureLoader;
	textureLoader.Load( fileListLoader.GetFileList().at( FileIOConstants::TextureList ) );
	Surfaces = textureLoader.GetTextures();

	FontLoader fontLoader( *Surfaces );
	fontLoader.Load( fileListLoader.GetFileList().at( FileIOConstants::FontList ) );
	Fonts = fontLoader.GetFonts();

	FixtureLoader fixtureLoader( *Surfaces );
	fixtureLoader.Load( fileListLoader.GetFileList().at( FileIOConstants::FixtureList ) );
	Fixtures = fixtureLoader.GetFixtures();

	MenuLoader menuLoader( _Editor.get(), *Fonts, *Fixtures, gfx );
	menuLoader.Load( fileListLoader.GetFileList().at( FileIOConstants::MenuList ) );
	MainMenuBar = menuLoader.GetMainMenuBar();

	MapLoader mapLoader( *Fixtures );
	mapLoader.Load( "Maps\\Map1.txt" );
	_Editor->SetMap( mapLoader.GetMap() );
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

void Game::ComposeFrame()
{
	_Editor->Draw( gfx );
	MainMenuBar->Draw();
}
