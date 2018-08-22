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

	LoadTextures();
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
	Settings.LoadSettings( "Settings\\Settings.txt" );
	Fonts->AddFont( "Font_Fixedsys16x28", std::make_unique<Font>( Surfaces->GetSurface( "Font_Fixedsys16x28" ), Colors::White, 28, Colors::White ) );
}

void Game::LoadTextures()
{
	Surfaces->AddSurface( "Font_Fixedsys16x28", std::make_unique<Surface>( "Textures\\Fonts\\Fixedsys16x28.bmp" ) );
	//Surfaces->AddSurface( "bluestone", std::make_unique<Surface>( "Textures\\bluestone.bmp" ) );
	//Surfaces->AddSurface( "colorstone", std::make_unique<Surface>( "Textures\\colorstone.bmp" ) );
	//Surfaces->AddSurface( "eagle", std::make_unique<Surface>( "Textures\\eagle.bmp" ) );
	//Surfaces->AddSurface( "greystone", std::make_unique<Surface>( "Textures\\greystone.bmp" ) );
	//Surfaces->AddSurface( "mossy", std::make_unique<Surface>( "Textures\\mossy.bmp" ) );
	//Surfaces->AddSurface( "purplestone", std::make_unique<Surface>( "Textures\\purplestone.bmp" ) );
	//Surfaces->AddSurface( "redbrick", std::make_unique<Surface>( "Textures\\redbrick.bmp" ) );
	//Surfaces->AddSurface( "wood", std::make_unique<Surface>( "Textures\\wood.bmp" ) );

	Surfaces->AddSurface( "wal00000", std::make_unique<Surface>( "Textures\\Walls\\Light\\wal00000.bmp" ) );
	Surfaces->AddSurface( "wal00001", std::make_unique<Surface>( "Textures\\Walls\\Dark\\wal00001.bmp" ) );
	Surfaces->AddSurface( "wal00002", std::make_unique<Surface>( "Textures\\Walls\\Light\\wal00002.bmp" ) );
	Surfaces->AddSurface( "wal00003", std::make_unique<Surface>( "Textures\\Walls\\Dark\\wal00003.bmp" ) );
}

void Game::SetupMenu()
{
	Font* menuFont = &Fonts->GetFont( "Font_Fixedsys16x28" );
	Editor* editor = _Editor.get();

	MainMenuBar = std::make_unique<MenuBar>( Vei2( 0, 0 ), Vei2( Graphics::ScreenWidth, 10 ), gfx );
	
	std::unique_ptr<MenuItem> editMenu = std::make_unique<Menu>( "Edit", menuFont, gfx );
	editMenu->AddMenuItem( "None", std::make_unique<Editor::LeftMouseClickEditModeCallBack>( editor, EditConstants::MouseLClickMode::None, nullptr ), editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::None ) );

	std::unique_ptr<MenuItem> insertItem = std::make_unique<MenuItem>( "Insert", nullptr, editMenu.get(), menuFont, gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	//TODO: Change this to use MapFixtures instead of straight Surface pointers for the callback
	// TODO: Also - hard-coded size of images? Maybe OK...
	//std::unique_ptr<MenuItem> blueStoneItem = std::make_unique<ImageMenuItem>( &Surfaces->GetSurface( "bluestone" ), 96, 96, std::make_unique<Editor::LeftMouseClickEditModeCallBack>( editor, EditConstants::MouseLClickMode::Insert, &Surfaces->GetSurface( "bluestone" ) ), insertItem.get(), gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	//std::unique_ptr<MenuItem> colorStoneItem = std::make_unique<ImageMenuItem>( &Surfaces->GetSurface( "colorstone" ), 96, 96, std::make_unique<Editor::LeftMouseClickEditModeCallBack>( editor, EditConstants::MouseLClickMode::Insert, &Surfaces->GetSurface( "colorstone" ) ), insertItem.get(), gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	//std::unique_ptr<MenuItem> eagleItem = std::make_unique<ImageMenuItem>( &Surfaces->GetSurface( "eagle" ), 96, 96, std::make_unique<Editor::LeftMouseClickEditModeCallBack>( editor, EditConstants::MouseLClickMode::Insert, &Surfaces->GetSurface( "eagle" ) ), insertItem.get(), gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	//std::unique_ptr<MenuItem> greyStoneItem = std::make_unique<ImageMenuItem>( &Surfaces->GetSurface( "greystone" ), 96, 96, std::make_unique<Editor::LeftMouseClickEditModeCallBack>( editor, EditConstants::MouseLClickMode::Insert, &Surfaces->GetSurface( "greystone" ) ), insertItem.get(), gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	//std::unique_ptr<MenuItem> mossyItem = std::make_unique<ImageMenuItem>( &Surfaces->GetSurface( "mossy" ), 96, 96, std::make_unique<Editor::LeftMouseClickEditModeCallBack>( editor, EditConstants::MouseLClickMode::Insert, &Surfaces->GetSurface( "mossy" ) ), insertItem.get(), gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	//std::unique_ptr<MenuItem> purpleStoneItem = std::make_unique<ImageMenuItem>( &Surfaces->GetSurface( "purplestone" ), 96, 96, std::make_unique<Editor::LeftMouseClickEditModeCallBack>( editor, EditConstants::MouseLClickMode::Insert, &Surfaces->GetSurface( "purplestone" ) ), insertItem.get(), gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	//std::unique_ptr<MenuItem> redBrickItem = std::make_unique<ImageMenuItem>( &Surfaces->GetSurface( "redbrick" ), 96, 96, std::make_unique<Editor::LeftMouseClickEditModeCallBack>( editor, EditConstants::MouseLClickMode::Insert, &Surfaces->GetSurface( "redbrick" ) ), insertItem.get(), gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	//std::unique_ptr<MenuItem> woodItem = std::make_unique<ImageMenuItem>( &Surfaces->GetSurface( "wood" ), 96, 96, std::make_unique<Editor::LeftMouseClickEditModeCallBack>( editor, EditConstants::MouseLClickMode::Insert, &Surfaces->GetSurface( "wood" ) ), insertItem.get(), gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	//insertItem->AddMenuItem( std::move( blueStoneItem ) );
	//insertItem->AddMenuItem( std::move( colorStoneItem ) );
	//insertItem->AddMenuItem( std::move( eagleItem ) );
	//insertItem->AddMenuItem( std::move( greyStoneItem ) );
	//insertItem->AddMenuItem( std::move( mossyItem ) );
	//insertItem->AddMenuItem( std::move( purpleStoneItem ) );
	//insertItem->AddMenuItem( std::move( redBrickItem ) );
	//insertItem->AddMenuItem( std::move( woodItem ) );

	std::unique_ptr<MenuItem> insertWallItem = std::make_unique<MenuItem>( "Wall", nullptr, insertItem.get(), menuFont, gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	std::unique_ptr<MenuItem> insertWallDarkItem = std::make_unique<MenuItem>( "Dark", nullptr, insertWallItem.get(), menuFont, gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	insertWallDarkItem->SetColumns( 3 );
	std::unique_ptr<MenuItem> insertWallLightItem = std::make_unique<MenuItem>( "Light", nullptr, insertWallItem.get(), menuFont, gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	insertWallLightItem->SetColumns( 3 );

	std::unique_ptr<MenuItem> wal00000Item = std::make_unique<ImageMenuItem>( &Surfaces->GetSurface( "wal00000" ), 96, 96, std::make_unique<Editor::LeftMouseClickEditModeCallBack>( editor, EditConstants::MouseLClickMode::Insert, &Surfaces->GetSurface( "wal00000" ) ), insertItem.get(), gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	insertWallLightItem->AddMenuItem( std::move( wal00000Item ) );
	std::unique_ptr<MenuItem> wal00001Item = std::make_unique<ImageMenuItem>( &Surfaces->GetSurface( "wal00001" ), 96, 96, std::make_unique<Editor::LeftMouseClickEditModeCallBack>( editor, EditConstants::MouseLClickMode::Insert, &Surfaces->GetSurface( "wal00001" ) ), insertItem.get(), gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	insertWallDarkItem->AddMenuItem( std::move( wal00001Item ) );
	std::unique_ptr<MenuItem> wal00002Item = std::make_unique<ImageMenuItem>( &Surfaces->GetSurface( "wal00002" ), 96, 96, std::make_unique<Editor::LeftMouseClickEditModeCallBack>( editor, EditConstants::MouseLClickMode::Insert, &Surfaces->GetSurface( "wal00002" ) ), insertItem.get(), gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	insertWallLightItem->AddMenuItem( std::move( wal00002Item ) );
	std::unique_ptr<MenuItem> wal00003Item = std::make_unique<ImageMenuItem>( &Surfaces->GetSurface( "wal00003" ), 96, 96, std::make_unique<Editor::LeftMouseClickEditModeCallBack>( editor, EditConstants::MouseLClickMode::Insert, &Surfaces->GetSurface( "wal00003" ) ), insertItem.get(), gfx, editor->GetCellHoverHighlightColour( EditConstants::MouseLClickMode::Insert ) );
	insertWallDarkItem->AddMenuItem( std::move( wal00003Item ) );

	insertWallItem->AddMenuItem( std::move( insertWallDarkItem ) );
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
