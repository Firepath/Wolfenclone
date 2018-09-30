#pragma once

#include <assert.h>
#include <string>

class LoadConstants
{
public:
	enum class ReadMode
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
		EnumOptionsCount // This is the size of the enum, don't add things after it
	};

	// TODO: Check how many of these are used and clean up

	static constexpr char Comment[] = "'";

	static constexpr char None[] = "[None]";

	static constexpr char ListFiles[] = "[List Files]";
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

	static constexpr char Font[] = "[Font]";
	static constexpr char MenuFont[] = "MenuFont";

	static constexpr char FixtureDoorDark[] = "[Fixture Door Dark]";
	static constexpr char FixtureDoorLight[] = "[Fixture Door Light]";
	static constexpr char FixtureTreasure[] = "[Fixture Treasure]";
	static constexpr char FixtureWallDark[] = "[Fixture Wall Dark]";
	static constexpr char FixtureWallLight[] = "[Fixture Wall Light]";

	static constexpr char FixtureMenu[] = "[Fixture Menu]";

	static std::string GetReadModeText( const ReadMode mode )
	{
		switch ( mode )
		{
		case ReadMode::None:
			return LoadConstants::None;
		case ReadMode::List_Files:
			return LoadConstants::ListFiles;
		case ReadMode::Texture_Door_Dark:
			return LoadConstants::TextureDoorDark;
		case ReadMode::Texture_Door_Light:
			return LoadConstants::TextureDoorLight;
		case ReadMode::Texture_Font:
			return LoadConstants::TextureFont;
		case ReadMode::Texture_Treasure:
			return LoadConstants::TextureTreasure;
		case ReadMode::Texture_Wall_Dark:
			return LoadConstants::TextureWallDark;
		case ReadMode::Texture_Wall_Light:
			return LoadConstants::TextureWallLight;
		case ReadMode::Font:
			return LoadConstants::Font;
		case ReadMode::Fixture_Door_Dark:
			return LoadConstants::FixtureDoorDark;
		case ReadMode::Fixture_Door_Light:
			return LoadConstants::FixtureDoorLight;
		case ReadMode::Fixture_Treasure:
			return LoadConstants::FixtureTreasure;
		case ReadMode::Fixture_Wall_Dark:
			return LoadConstants::FixtureWallDark;
		case ReadMode::Fixture_Wall_Light:
			return LoadConstants::FixtureWallLight;
		case ReadMode::Fixture_Menu:
			return LoadConstants::FixtureMenu;
		}

		// Make sure we ALWAYS handle each mode's text.
		assert( false );

		return std::string();
	}

	static bool UpdateReadMode( std::string line, ReadMode& currentMode )
	{
		// If a blank line or a comment line, skip it.
		if ( line.length() == 0 || line.find( LoadConstants::Comment, 0 ) == 0 )
		{
			return true;
		}

		bool setMode = false;
		for ( int mode = (int)LoadConstants::ReadMode::None; mode != (int)LoadConstants::ReadMode::EnumOptionsCount; mode++ )
		{
			if ( line.find( LoadConstants::GetReadModeText( (LoadConstants::ReadMode)mode ), 0 ) == 0 )
			{
				currentMode = (LoadConstants::ReadMode)mode;
				setMode = true;
				break;
			}
		}

		return setMode;
	}
};
