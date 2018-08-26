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

#include "EditTool.h"
#include "Font.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	_Editor = std::make_unique<Editor>();

	_Settings = std::make_unique<Settings>();
	Surfaces = std::make_unique<StringKeyRepository<Surface>>();
	Fonts = std::make_unique<StringKeyRepository<Font>>();
	Fixtures = std::make_unique<StringKeyRepository<MapFixture>>();

	_Settings->LoadSettings( "Settings\\Settings.txt" );
	LoadTextures();
	LoadFonts();
	LoadFixtures();
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

void Game::FillFixtureMenuItems( std::unique_ptr<MenuItem>& menuItem, Editor* const editor, const Settings::ReadMode fixtureContents )
{
	EditTool_MouseButton* tool = editor->GetToolBox().GetMouseButtonTool( EditTool_MouseButton_Insert::TypeName );
	auto& listFixtures = _Settings->GetSettingList( fixtureContents );
	for ( auto it = listFixtures.begin(); it != listFixtures.end(); it++ )
	{
		MapFixture* fixture = &(Fixtures->GetItem( it->first ));

		std::unique_ptr<MenuItem> subMenuItem = std::make_unique<ImageMenuItem>( fixture->GetTexture(), 64, 64, std::make_unique<Editor::EditTool_MouseButton_InsertLCallBack>( editor, tool, fixture ), menuItem.get(), gfx, tool->GetToolColour() );
		menuItem->AddMenuItem( std::move( subMenuItem ) );
	}
}

void Game::LoadFonts()
{
	Fonts->AddItem( "Font_Fixedsys16x28", std::make_unique<Font>( Surfaces->GetItem( "Font_Fixedsys16x28" ), Colors::White, 14, Colors::White ) );
}

void Game::LoadFixtures()
{
	LoadFixtures( Settings::ReadMode::Texture_Wall_Dark );
	LoadFixtures( Settings::ReadMode::Texture_Wall_Light );
}

void Game::LoadFixtures( const Settings::ReadMode contents )
{
	auto& listFixtures = _Settings->GetSettingList( contents );
	for ( auto it = listFixtures.begin(); it != listFixtures.end(); it++ )
	{
		const Surface* texture = &(Surfaces->GetItem( it->first ));
		std::unique_ptr<MapFixture> wall = std::make_unique<Wall>( texture );
		Fixtures->AddItem( it->first, std::move( wall ) );
	}
}

void Game::LoadTextures()
{
	Surfaces->AddItem( "Font_Fixedsys16x28", std::make_unique<Surface>( "Textures\\Fonts\\Fixedsys16x28.bmp" ) );
	LoadTextures( Settings::ReadMode::Texture_Wall_Dark );
	LoadTextures( Settings::ReadMode::Texture_Wall_Light );
}

void Game::LoadTextures( const Settings::ReadMode contents )
{
	auto& listTextures = _Settings->GetSettingList( contents );
	for ( auto it = listTextures.begin(); it != listTextures.end(); it++ )
	{
		Surfaces->AddItem( it->first, std::make_unique<Surface>( it->second ) );
	}
}

void Game::SetupMenu()
{
	Font* menuFont = &Fonts->GetItem( "Font_Fixedsys16x28" );
	Editor* editor = _Editor.get();

	MainMenuBar = std::make_unique<MenuBar>( Vei2( 0, 0 ), Vei2( Graphics::ScreenWidth, 10 ), gfx );
	
	std::unique_ptr<MenuItem> editMenu = std::make_unique<Menu>( "Edit", menuFont, gfx );

	EditTool_MouseButton* noneTool = editor->GetToolBox().GetMouseButtonTool( EditTool_MouseButton_None::TypeName );
	editMenu->AddMenuItem( "None", std::make_unique<Editor::EditTool_MouseButtonLCallBack>( editor, noneTool ), noneTool->GetToolColour() );

	Color insertToolColour = editor->GetToolBox().GetMouseButtonTool( EditTool_MouseButton_Insert::TypeName )->GetToolColour();
	std::unique_ptr<MenuItem> insertItem = std::make_unique<MenuItem>( "Insert", nullptr, editMenu.get(), menuFont, gfx, insertToolColour );
	std::unique_ptr<MenuItem> insertWallItem = std::make_unique<MenuItem>( "Wall", nullptr, insertItem.get(), menuFont, gfx, insertToolColour );

	std::unique_ptr<MenuItem> insertWallDarkItem = std::make_unique<MenuItem>( "Dark", nullptr, insertWallItem.get(), menuFont, gfx, insertToolColour );
	insertWallDarkItem->SetColumns( 10 );
	FillFixtureMenuItems( insertWallDarkItem, editor, Settings::ReadMode::Texture_Wall_Dark );
	insertWallItem->AddMenuItem( std::move( insertWallDarkItem ) );

	std::unique_ptr<MenuItem> insertWallLightItem = std::make_unique<MenuItem>( "Light", nullptr, insertWallItem.get(), menuFont, gfx, insertToolColour );
	insertWallLightItem->SetColumns( 10 );
	FillFixtureMenuItems( insertWallLightItem, editor, Settings::ReadMode::Texture_Wall_Light );
	insertWallItem->AddMenuItem( std::move( insertWallLightItem ) );

	insertItem->AddMenuItem( std::move( insertWallItem ) );
	editMenu->AddMenuItem( std::move( insertItem ) );
	
	EditTool_MouseButton* selectTool = editor->GetToolBox().GetMouseButtonTool( EditTool_MouseButton_Select::TypeName );
	editMenu->AddMenuItem( "Select", std::make_unique<Editor::EditTool_MouseButtonLCallBack>( editor, selectTool ), selectTool->GetToolColour() );

	EditTool_MouseButton* moveTool = editor->GetToolBox().GetMouseButtonTool( EditTool_MouseButton_Move::TypeName );
	std::unique_ptr<SelectedCallBack> moveCallBack = std::make_unique<Editor::EditTool_MouseButtonLCallBack>( editor, moveTool );
	editMenu->AddMenuItem( "Move", std::move( moveCallBack ), moveTool->GetToolColour() );

	MainMenuBar->AddMenu( std::move( editMenu ) );
}

void Game::ComposeFrame()
{
	_Editor->Draw( gfx );
	MainMenuBar->Draw();
}
