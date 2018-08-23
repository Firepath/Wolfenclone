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

#include "Font.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	_Editor = std::make_unique<Editor>();

	Surfaces = std::make_unique<SurfaceRepository>();
	Fonts = std::make_unique<FontRepository>();

	Settings.LoadSettings( "Settings\\Settings.txt" );
	LoadTextures();
	LoadFonts();
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

void Game::FillTextureMenuItemSubMenu( std::unique_ptr<MenuItem>& menuItem, Editor* const editor, const Settings::ReadMode contents )
{
	auto& listTextures = Settings.GetSettingList( contents );
	for ( auto it = listTextures.begin(); it != listTextures.end(); it++ )
	{
		// TODO: Change this to use MapFixtures instead of straight Surface pointers for the callback
		// TODO: Also - hard-coded size of images? Maybe OK...
		std::unique_ptr<MenuItem> subMenuItem = std::make_unique<ImageMenuItem>( &Surfaces->GetSurface( it->first ), 64, 64, std::make_unique<Editor::LeftMouseClickEditModeCallBack>( editor, EditConstants::MouseLClickMode::Insert, &Surfaces->GetSurface( it->first ) ), menuItem.get(), gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
		menuItem->AddMenuItem( std::move( subMenuItem ) );
	}
}

void Game::LoadFonts()
{
	Fonts->AddFont( "Font_Fixedsys16x28", std::make_unique<Font>( Surfaces->GetSurface( "Font_Fixedsys16x28" ), Colors::White, 14, Colors::White ) );
}

void Game::LoadTextures()
{
	Surfaces->AddSurface( "Font_Fixedsys16x28", std::make_unique<Surface>( "Textures\\Fonts\\Fixedsys16x28.bmp" ) );
	LoadTextures( Settings::ReadMode::Texture_Wall_Dark );
	LoadTextures( Settings::ReadMode::Texture_Wall_Light );
}

void Game::LoadTextures( const Settings::ReadMode contents )
{
	auto& listTextures = Settings.GetSettingList( contents );
	for ( auto it = listTextures.begin(); it != listTextures.end(); it++ )
	{
		Surfaces->AddSurface( it->first, std::make_unique<Surface>( it->second ) );
	}
}

void Game::SetupMenu()
{
	Font* menuFont = &Fonts->GetFont( "Font_Fixedsys16x28" );
	Editor* editor = _Editor.get();

	MainMenuBar = std::make_unique<MenuBar>( Vei2( 0, 0 ), Vei2( Graphics::ScreenWidth, 10 ), gfx );
	
	std::unique_ptr<MenuItem> editMenu = std::make_unique<Menu>( "Edit", menuFont, gfx );
	editMenu->AddMenuItem( "None", std::make_unique<Editor::LeftMouseClickEditModeCallBack>( editor, EditConstants::MouseLClickMode::None, nullptr ), editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::None ) );
	std::unique_ptr<MenuItem> insertItem = std::make_unique<MenuItem>( "Insert", nullptr, editMenu.get(), menuFont, gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	std::unique_ptr<MenuItem> insertWallItem = std::make_unique<MenuItem>( "Wall", nullptr, insertItem.get(), menuFont, gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );

	std::unique_ptr<MenuItem> insertWallDarkItem = std::make_unique<MenuItem>( "Dark", nullptr, insertWallItem.get(), menuFont, gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	insertWallDarkItem->SetColumns( 10 );
	FillTextureMenuItemSubMenu( insertWallDarkItem, editor, Settings::ReadMode::Texture_Wall_Dark );
	insertWallItem->AddMenuItem( std::move( insertWallDarkItem ) );

	std::unique_ptr<MenuItem> insertWallLightItem = std::make_unique<MenuItem>( "Light", nullptr, insertWallItem.get(), menuFont, gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	insertWallLightItem->SetColumns( 10 );
	FillTextureMenuItemSubMenu( insertWallLightItem, editor, Settings::ReadMode::Texture_Wall_Light );
	insertWallItem->AddMenuItem( std::move( insertWallLightItem ) );

	insertItem->AddMenuItem( std::move( insertWallItem ) );
	editMenu->AddMenuItem( std::move( insertItem ) );
	
	editMenu->AddMenuItem( "Select", std::make_unique<Editor::LeftMouseClickEditModeCallBack>( editor, EditConstants::MouseLClickMode::Select, nullptr ), editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Select ) );
	
	MainMenuBar->AddMenu( std::move( editMenu ) );
}

void Game::ComposeFrame()
{
	_Editor->Draw( gfx );
	MainMenuBar->Draw();
}
