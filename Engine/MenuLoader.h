#pragma once

#include <fstream>
#include <vector>
#include <string>

#include "LoadConstants.h"
#include "Loader.h"
#include "StringKeyRepository.h"

class Editor;
class Font;
class Graphics;
class MapFixture;
class MenuBar;
class MenuItem;

class MenuStructure
{
public:
	MenuStructure( const std::string name, const std::string text, const std::string parent, StringKeyRepository<MapFixture>* items, const size_t columns = 1 )
		:
		Name( name ),
		Text( text ),
		Parent( parent ),
		Items( items ),
		Columns( columns )
	{
	}

	const std::string Name;
	const std::string Text;
	const std::string Parent;
	StringKeyRepository<MapFixture>* Items = nullptr;
	size_t Columns;
};

inline bool operator==( const MenuStructure lhs, const std::string rhs )
{
	return lhs.Name == rhs;
}

class MenuLoader : public Loader
{
public:
	MenuLoader( Editor* editor, StringKeyRepository<Font>& fonts, StringKeyRepository<StringKeyRepository<MapFixture>>& fixtures, Graphics& gfx );

	std::unique_ptr<MenuBar> GetMainMenuBar();

	void Load( const std::string& filename ) override;

private:
	std::string FindFirstInLineTextAndRemoveAndReturn( std::string& line, const char startDelimiter, const char endDelimiter, bool includeDelimiters = false );
	void ReadSetting( const std::string& line );

	Editor* _Editor;
	StringKeyRepository<Font>& Fonts;
	StringKeyRepository<StringKeyRepository<MapFixture>>& Fixtures;
	std::vector<MenuStructure> FixtureMenuStructure;
	Graphics& _Graphics;
	std::unique_ptr<MenuBar> MainMenuBar = nullptr;
};