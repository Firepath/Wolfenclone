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

void Game::LoadFonts()
{
	Fonts->AddItem( "Font_Fixedsys16x28", std::make_unique<Font>( Surfaces->GetItem( "Font_Fixedsys16x28" ), Colors::White, 14, Colors::White ) );
}

void Game::LoadFixtures()
{
	LoadFixtures<Wall>( Settings::ReadMode::Map_Fixture_Door_Dark );
	LoadFixtures<Wall>( Settings::ReadMode::Map_Fixture_Door_Light );
	LoadFixtures<Treasure>( Settings::ReadMode::Map_Fixture_Treasure );
	LoadFixtures<Wall>( Settings::ReadMode::Map_Fixture_Wall_Dark );
	LoadFixtures<Wall>( Settings::ReadMode::Map_Fixture_Wall_Light );
}

void Game::LoadTextures()
{
	Surfaces->AddItem( "Font_Fixedsys16x28", std::make_unique<Surface>( "Textures\\Fonts\\Fixedsys16x28.bmp" ) );
	LoadTextures( Settings::ReadMode::Texture_Door_Dark );
	LoadTextures( Settings::ReadMode::Texture_Door_Light );
	LoadTextures( Settings::ReadMode::Texture_Treasure );
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

	// This whole thing can probably be put in a method (for re-use), and given the insertMenu (or another) as the highest-level item.

	std::unordered_map<std::string, std::unique_ptr<MenuItem>, std::hash<std::string>> menuItemCollection;
	const std::vector<MenuStructure>& structure = _Settings->GetFixtureMenuStructure();
	for ( auto it = structure.begin(); it != structure.end(); it++ )
	{
		if ( menuItemCollection.find( it->Name ) == menuItemCollection.end() )
		{
			const MenuItem* parentMenu = insertItem.get();
			if ( !it->Parent.empty() )
			{
				// This logic requires menus be created in genealogical order (parents, then children)
				parentMenu = menuItemCollection.at( it->Parent ).get();
			}

			menuItemCollection[it->Name] = std::make_unique<MenuItem>( it->Text, nullptr, parentMenu, menuFont, gfx, insertToolColour );
		}

		MenuItem* const menuItem = menuItemCollection.at( it->Name ).get();

		if ( it->Columns > 1 )
		{
			menuItem->SetColumns( it->Columns );
		}

		if ( it->Items.size() > 0 )
		{
			EditTool_MouseButton* tool = editor->GetToolBox().GetMouseButtonTool( EditTool_MouseButton_Insert::TypeName );
			for ( const std::string& fixtureName : it->Items )
			{
				MapFixture* fixture = &(Fixtures->GetItem( fixtureName ));
				const Surface* surface = fixture->GetTexture();

				std::unique_ptr<MenuItem> subMenuItem = std::make_unique<ImageMenuItem>( surface, surface->GetHeight(), surface->GetWidth(), std::make_unique<Editor::EditTool_MouseButton_InsertLCallBack>( editor, tool, fixture ), menuItem, gfx, tool->GetToolColour() );
				menuItem->AddMenuItem( std::move( subMenuItem ) );
			}
		}
	}

	for ( auto it = structure.rbegin(); it != structure.rend(); it++ )
	{
		std::unique_ptr<MenuItem> child = std::move( menuItemCollection.at( it->Name ) );

		if ( !it->Parent.empty() )
		{
			MenuItem* const parent = menuItemCollection.at( it->Parent ).get();
			parent->StackMenuItem( std::move( child ) );
		}
		else
		{
			insertItem->StackMenuItem( std::move( child ) );
		}
	}

	editMenu->AddMenuItem( std::move( insertItem ) );
	
	EditTool_MouseButton* deleteTool = editor->GetToolBox().GetMouseButtonTool( EditTool_MouseButton_Delete::TypeName );
	editMenu->AddMenuItem( "Delete", std::make_unique<Editor::EditTool_MouseButtonLCallBack>( editor, deleteTool ), deleteTool->GetToolColour() );

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
