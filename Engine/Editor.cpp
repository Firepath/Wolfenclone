#include "Editor.h"

Editor::Editor()
	:
	MapGrid( 180, 100, Vec2( 0.0f, 0.0f ) )
{
}

void Editor::DoMouseEvents( const Mouse::Event & me )
{
	MapGrid.DoMouseEvents( me );
}

void Editor::Draw( Graphics & gfx )
{
	MapGrid.Draw( gfx );
}
