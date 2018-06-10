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
#include "SpriteEffect.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
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

void Game::ComposeFrame()
{
	Editor.Draw( gfx );

	//gfx.DrawSprite( RectI( 100, 228, 100, 228 ), greyWall.GetRect(), Graphics::GetScreenRect(), greyWall, SpriteEffect::Copy() );
	//gfx.DrawSprite( RectI( 100, 164, 100, 164 ), greyWall.GetRect(), Graphics::GetScreenRect(), greyWall, SpriteEffect::Copy() );
	//gfx.DrawSprite( RectI( 100, 132, 100, 132 ), greyWall.GetRect(), Graphics::GetScreenRect(), greyWall, SpriteEffect::Copy() );
	gfx.DrawSprite( RectI( 100, 228, 100, 228 ), greyWall, SpriteEffect::Copy() );
	gfx.DrawSprite( RectI( 100, 164, 100, 164 ), greyWall.GetRect(), greyWall, SpriteEffect::Copy() );
	gfx.DrawSprite( RectI( 100, 132, 100, 132 ), greyWall.GetRect(), Graphics::GetScreenRect(), greyWall, SpriteEffect::Copy() );
}
