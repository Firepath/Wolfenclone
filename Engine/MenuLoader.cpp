#include "MenuLoader.h"
#include "Colors.h"
#include "Editor.h"
#include "EditTool.h"
#include "Graphics.h"
#include "MapFixture.h"
#include "Menu.h"
#include "Vec2.h"

MenuLoader::MenuLoader( Editor* editor, StringKeyRepository<Font>& fonts, StringKeyRepository<StringKeyRepository<MapFixture>>& fixtures, Graphics & gfx )
	:
	_Editor( editor ),
	Fonts( fonts ),
	Fixtures( fixtures ),
	_Graphics( gfx )
{
}

std::unique_ptr<MenuBar> MenuLoader::GetMainMenuBar()
{
	return std::move( MainMenuBar );
}

void MenuLoader::Load( const std::string& filename )
{
	MainMenuBar = std::make_unique<MenuBar>( Vei2( 0, 0 ), Vei2( Graphics::ScreenWidth, 10 ), _Graphics );

	Font* menuFont = &(Fonts.GetItem( LoadConstants::MenuFont ));
	Editor* editor = _Editor;

	std::unique_ptr<MenuItem> editMenu = std::make_unique<Menu>( "Edit", menuFont, _Graphics );

	EditTool_MouseButton* noneTool = editor->GetToolBox().GetMouseButtonTool( EditTool_MouseButton_None::TypeName );
	editMenu->AddMenuItem( "None", std::make_unique<Editor::EditTool_MouseButtonLCallBack>( editor, noneTool ), noneTool->GetToolColour() );

	Color insertToolColour = editor->GetToolBox().GetMouseButtonTool( EditTool_MouseButton_Insert::TypeName )->GetToolColour();
	std::unique_ptr<MenuItem> insertItem = std::make_unique<MenuItem>( "Insert", nullptr, editMenu.get(), menuFont, _Graphics, insertToolColour );

	Loader::Load( filename );

	std::unordered_map<std::string, std::unique_ptr<MenuItem>, std::hash<std::string>> menuItemCollection;
	const std::vector<MenuStructure>& structure = FixtureMenuStructure;
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

			menuItemCollection[it->Name] = std::make_unique<MenuItem>( it->Text, nullptr, parentMenu, menuFont, _Graphics, insertToolColour );
		}

		MenuItem* const menuItem = menuItemCollection.at( it->Name ).get();

		if ( it->Columns > 1 )
		{
			menuItem->SetColumns( it->Columns );
		}

		if ( it->Items != nullptr )
		{
			EditTool_MouseButton* tool = editor->GetToolBox().GetMouseButtonTool( EditTool_MouseButton_Insert::TypeName );
			for ( const std::string& fixtureName : it->Items->GetKeys() )
			{
				MapFixture* fixture = &(it->Items->GetItem( fixtureName ));
				const Surface* surface = fixture->GetTexture();

				std::unique_ptr<MenuItem> subMenuItem = std::make_unique<ImageMenuItem>( surface, surface->GetHeight(), surface->GetWidth(), std::make_unique<Editor::EditTool_MouseButton_InsertLCallBack>( editor, tool, fixture ), menuItem, _Graphics, tool->GetToolColour() );
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

std::string MenuLoader::FindFirstInLineTextAndRemoveAndReturn( std::string& line, const char startDelimiter, const char endDelimiter, bool includeDelimiters )
{
	size_t start = line.find( startDelimiter );
	size_t end = line.find( endDelimiter );
	if ( start == std::string::npos ||
		end == std::string::npos ||
		end == 0 ||
		start >= end ) // Equal-to if same delimiter for start and end
	{
		return std::string();
	}

	if ( !includeDelimiters )
	{
		start += 1;
	}
	else
	{
		end += 1;
	}

	end -= start;

	std::string text = line.substr( start, end );
	line = line.substr( end + 2, line.size() - (end + 2) );
	return text;
}

void MenuLoader::ReadSetting( const std::string& line )
{
	switch ( Mode )
	{
	case LoadConstants::ReadMode::Fixture_Menu:
	{
		std::string lineCopy = line;
		const std::string text = FindFirstInLineTextAndRemoveAndReturn( lineCopy, '(', ')' );
		const std::string parent = FindFirstInLineTextAndRemoveAndReturn( lineCopy, '(', ')' );
		const std::string fixtureList = FindFirstInLineTextAndRemoveAndReturn( lineCopy, '[', ']', true );
		const std::string columns = FindFirstInLineTextAndRemoveAndReturn( lineCopy, '[', ']' );
		const std::string name = parent + text;

		if ( std::find( FixtureMenuStructure.begin(), FixtureMenuStructure.end(), name ) == FixtureMenuStructure.end() )
		{
			int cols = 1;
			if ( !columns.empty() )
			{
				cols = std::stoi( columns );
			}

			StringKeyRepository<MapFixture>* fixture = nullptr;
			if ( !fixtureList.empty() )
			{
				fixture = &(Fixtures.GetItem( fixtureList ));
			}

			FixtureMenuStructure.emplace_back( name, text, parent, fixture, cols );
		}
	}
	break;
	}
}
