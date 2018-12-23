#pragma once

#include <assert.h>
#include <string>

class FileIOConstants
{
public:
	enum class IOMode
	{
		None,
		List_Files,
		Texture_Door_Dark,
		Texture_Door_Light,
		Texture_Font,
		Texture_Treasure,
		Texture_Wall_Dark,
		Texture_Wall_Light,
		Font,
		Fixture_Door_Dark,
		Fixture_Door_Light,
		Fixture_Treasure,
		Fixture_Wall_Dark,
		Fixture_Wall_Light,
		Fixture_Menu,
		Map_Name,
		Map_Cell_Content,
		EnumOptionsCount // This is the size of the enum, don't add things after it
	};

	// TODO: Check how many of these are used and clean up

	static constexpr char Comment[] = "'";

	static constexpr char None[] = "[None]";

	static constexpr char ListFiles[] = "[List Files]";
	static constexpr char ListFilesComment[] = "'{List Name} {Relative File Path}";
	static constexpr char TextureList[] = "Textures";
	static constexpr char FontList[] = "Fonts";
	static constexpr char FixtureList[] = "Fixtures";
	static constexpr char MenuList[] = "Menus";

	static constexpr char TextureDoorDark[] = "[Texture Door Dark]";
	static constexpr char TextureDoorLight[] = "[Texture Door Light]";
	static constexpr char TextureFont[] = "[Texture Font]";
	static constexpr char TextureTreasure[] = "[Texture Treasure]";
	static constexpr char TextureWallDark[] = "[Texture Wall Dark]";
	static constexpr char TextureWallLight[] = "[Texture Wall Light]";
	static constexpr char TextureComment[] = "'{Texture Name} {Relative File Path}";

	static constexpr char Font[] = "[Font]";
	static constexpr char FontComment[] = "'{Font Name} {Texture Name}";
	static constexpr char MenuFont[] = "MenuFont";

	static constexpr char FixtureDoorDark[] = "[Fixture Door Dark]";
	static constexpr char FixtureDoorLight[] = "[Fixture Door Light]";
	static constexpr char FixtureDoorComment[] = "'{Fixture Name} {Texture Name}";
	static constexpr char FixtureTreasure[] = "[Fixture Treasure]";
	static constexpr char FixtureTreasureComment[] = "'{Fixture Name} {Texture Name}";
	static constexpr char FixtureWallDark[] = "[Fixture Wall Dark]";
	static constexpr char FixtureWallLight[] = "[Fixture Wall Light]";
	static constexpr char FixtureWallComment[] = "'{Fixture Name} {Texture Name}";

	static constexpr char FixtureMenu[] = "[Fixture Menu]";
	static constexpr char FixtureMenuComment[] = "'[(Menu Name)] {(Parent Menu Name)} {[Fixture Type]} {[Number of Columns]}";

	static constexpr char MapName[] = "[Map Name]";
	static constexpr char MapNameComment[] = "'{Map Name}";
	static constexpr char MapCellContent[] = "[Map Cell Content]";
	static constexpr char MapCellContentComment[] = "'{Cell Location} {Fixture Type} {Fixture Name}";

	static std::string GetIOModeText( const IOMode mode )
	{
		switch ( mode )
		{
		case IOMode::None:
			return FileIOConstants::None;
		case IOMode::List_Files:
			return FileIOConstants::ListFiles;
		case IOMode::Texture_Door_Dark:
			return FileIOConstants::TextureDoorDark;
		case IOMode::Texture_Door_Light:
			return FileIOConstants::TextureDoorLight;
		case IOMode::Texture_Font:
			return FileIOConstants::TextureFont;
		case IOMode::Texture_Treasure:
			return FileIOConstants::TextureTreasure;
		case IOMode::Texture_Wall_Dark:
			return FileIOConstants::TextureWallDark;
		case IOMode::Texture_Wall_Light:
			return FileIOConstants::TextureWallLight;
		case IOMode::Font:
			return FileIOConstants::Font;
		case IOMode::Fixture_Door_Dark:
			return FileIOConstants::FixtureDoorDark;
		case IOMode::Fixture_Door_Light:
			return FileIOConstants::FixtureDoorLight;
		case IOMode::Fixture_Treasure:
			return FileIOConstants::FixtureTreasure;
		case IOMode::Fixture_Wall_Dark:
			return FileIOConstants::FixtureWallDark;
		case IOMode::Fixture_Wall_Light:
			return FileIOConstants::FixtureWallLight;
		case IOMode::Fixture_Menu:
			return FileIOConstants::FixtureMenu;
		case IOMode::Map_Name:
			return FileIOConstants::MapName;
		case IOMode::Map_Cell_Content:
			return FileIOConstants::MapCellContent;
		}

		// Make sure we ALWAYS handle each mode's text.
		assert( false );

		return std::string();
	}

	static std::string GetIOModeCommentText( const IOMode mode )
	{
		switch ( mode )
		{
		case IOMode::None:
			return FileIOConstants::None;
		case IOMode::List_Files:
			return FileIOConstants::ListFilesComment;
		case IOMode::Texture_Door_Dark:
		case IOMode::Texture_Door_Light:
		case IOMode::Texture_Font:
		case IOMode::Texture_Treasure:
		case IOMode::Texture_Wall_Dark:
		case IOMode::Texture_Wall_Light:
			return FileIOConstants::TextureComment;
		case IOMode::Font:
			return FileIOConstants::FontComment;
		case IOMode::Fixture_Door_Dark:
		case IOMode::Fixture_Door_Light:
			return FileIOConstants::FixtureDoorComment;
		case IOMode::Fixture_Treasure:
			return FileIOConstants::FixtureTreasureComment;
		case IOMode::Fixture_Wall_Dark:
		case IOMode::Fixture_Wall_Light:
			return FileIOConstants::FixtureWallComment;
		case IOMode::Fixture_Menu:
			return FileIOConstants::FixtureMenuComment;
		case IOMode::Map_Name:
			return FileIOConstants::MapNameComment;
		case IOMode::Map_Cell_Content:
			return FileIOConstants::MapCellContentComment;
		}

		// Make sure we ALWAYS handle each mode's text.
		assert( false );

		return std::string();
	}
};
