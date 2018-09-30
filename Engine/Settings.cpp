#include <algorithm>
#include <assert.h>

#include "Settings.h"

Settings::Settings()
{
	SettingsLists[Settings::ListFiles] = std::make_unique<std::unordered_map<std::string, std::string, std::hash<std::string>>>();

	SettingsLists[Settings::TextureDoorDark] = std::make_unique<std::unordered_map<std::string, std::string, std::hash<std::string>>>();
	SettingsLists[Settings::TextureDoorLight] = std::make_unique<std::unordered_map<std::string, std::string, std::hash<std::string>>>();
	SettingsLists[Settings::TextureTreasure] = std::make_unique<std::unordered_map<std::string, std::string, std::hash<std::string>>>();
	SettingsLists[Settings::TextureWallDark] = std::make_unique<std::unordered_map<std::string, std::string, std::hash<std::string>>>();
	SettingsLists[Settings::TextureWallLight] = std::make_unique<std::unordered_map<std::string, std::string, std::hash<std::string>>>();

	SettingsLists[Settings::MapFixtureDoorDark] = std::make_unique<std::unordered_map<std::string, std::string, std::hash<std::string>>>();
	SettingsLists[Settings::MapFixtureDoorLight] = std::make_unique<std::unordered_map<std::string, std::string, std::hash<std::string>>>();
	SettingsLists[Settings::MapFixtureTreasure] = std::make_unique<std::unordered_map<std::string, std::string, std::hash<std::string>>>();
	SettingsLists[Settings::MapFixtureWallDark] = std::make_unique<std::unordered_map<std::string, std::string, std::hash<std::string>>>();
	SettingsLists[Settings::MapFixtureWallLight] = std::make_unique<std::unordered_map<std::string, std::string, std::hash<std::string>>>();
	SettingsLists[Settings::MapFixtureMenu] = std::make_unique<std::unordered_map<std::string, std::string, std::hash<std::string>>>();
}

const std::unordered_map<std::string, std::string, std::hash<std::string>>& Settings::GetSettingList( ReadMode mode )
{
	return *(SettingsLists.at( GetReadModeText( mode ) ).get());
}

const std::vector<MenuStructure>& Settings::GetFixtureMenuStructure()
{
	return FixtureMenuStructure;
}

void Settings::LoadSettings( const std::string& filename )
{
	_LoadSettings( filename );

	auto& listFiles = *(SettingsLists.at( Settings::ListFiles ).get());
	for ( auto it = listFiles.begin(); it != listFiles.end(); it++ )
	{
		_LoadSettings( it->second );
	}

	FinaliseSettings();
}

void Settings::FinaliseSettings()
{
	// Ensure all settings are within acceptable bounds (which will also default any un-initialised settings).
	//StartingNumberOfTargets = std::min( MaxStartingNumberOfTargets, std::max( MinStartingNumberOfTargets, StartingNumberOfTargets ) );
}

std::string Settings::FindFirstInLineTextAndRemoveAndReturn( std::string& line, const char startDelimiter, const char endDelimiter, bool includeDelimiters )
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

void Settings::_LoadSettings( const std::string& filename )
{
	std::string line;
	std::ifstream in( filename.c_str() );

	while ( std::getline( in, line ).good() )
	{
		// If a blank line or a comment line, skip it.
		if ( line.length() == 0 || line.find( Settings::Comment, 0 ) == 0 )
		{
			continue;
		}
		else
		{
			bool changedMode = false;
			for ( int mode = (int)ReadMode::None; mode != (int)ReadMode::EnumOptionsCount; mode++ )
			{
				if ( line.find( GetReadModeText( (ReadMode)mode ), 0 ) == 0 )
				{
					Mode = (ReadMode)mode;
					changedMode = true;
					break;
				}
			}

			if ( changedMode )
			{
				continue;
			}
		}

		ReadSetting( line );
	}

	Mode = ReadMode::None;
}

void Settings::ReadSetting( const std::string& line )
{
	switch ( Mode )
	{
	case ReadMode::List_Files:
	case ReadMode::Texture_Door_Dark:
	case ReadMode::Texture_Door_Light:
	case ReadMode::Texture_Treasure:
	case ReadMode::Texture_Wall_Dark:
	case ReadMode::Texture_Wall_Light:
	{
		size_t split = line.find( " " );
		std::string name = line.substr( 0, split );
		std::string filename = line.substr( split + 1, line.length() - split );

		auto& list = *(SettingsLists.at( GetReadModeText( Mode ) ).get());
		list[name] = filename;
	}
		break;
	case ReadMode::Map_Fixture_Door_Dark:
	case ReadMode::Map_Fixture_Door_Light:
	case ReadMode::Map_Fixture_Treasure:
	case ReadMode::Map_Fixture_Wall_Dark:
	case ReadMode::Map_Fixture_Wall_Light:
	{
		size_t split = line.find( " " );
		std::string name = line.substr( 0, split );
		std::string textureName = line.substr( split + 1, line.length() - split );

		auto& list = *(SettingsLists.at( GetReadModeText( Mode ) ).get());
		list[name] = textureName;
	}
		break;
	case ReadMode::Map_Fixture_Menu:
	{
		std::string lineCopy = line;
		const std::string text = FindFirstInLineTextAndRemoveAndReturn( lineCopy, '(', ')' );
		const std::string parent = FindFirstInLineTextAndRemoveAndReturn( lineCopy, '(', ')' );
		const std::string fixtureList = FindFirstInLineTextAndRemoveAndReturn( lineCopy, '[', ']', true );
		const std::string columns = FindFirstInLineTextAndRemoveAndReturn( lineCopy, '[', ']' );
		const std::string name = parent + text;

		if ( std::find( FixtureMenuStructure.begin(), FixtureMenuStructure.end(), name ) == FixtureMenuStructure.end() )
		{
			FixtureMenuStructure.emplace_back( name, text, parent );
		}
		
		MenuStructure& menu = *(std::find( FixtureMenuStructure.begin(), FixtureMenuStructure.end(), name ));

		if ( !columns.empty() )
		{
			menu.Columns = std::stoi( columns );
		}

		if ( !fixtureList.empty() )
		{
			auto& list = *(SettingsLists.at( fixtureList ).get());
			for ( auto it = list.begin(); it != list.end(); it++ )
			{
				menu.Items.push_back( it->first );
			}
		}
	}
		break;
	}
}

std::string Settings::GetReadModeText( const ReadMode mode ) const
{
	switch ( mode )
	{
	case ReadMode::None:
		return Settings::None;
	case ReadMode::List_Files:
		return Settings::ListFiles;
	case ReadMode::Texture_Door_Dark:
		return Settings::TextureDoorDark;
	case ReadMode::Texture_Door_Light:
		return Settings::TextureDoorLight;
	case ReadMode::Texture_Treasure:
		return Settings::TextureTreasure;
	case ReadMode::Texture_Wall_Dark:
		return Settings::TextureWallDark;
	case ReadMode::Texture_Wall_Light:
		return Settings::TextureWallLight;
	case ReadMode::Map_Fixture_Door_Dark:
		return Settings::MapFixtureDoorDark;
	case ReadMode::Map_Fixture_Door_Light:
		return Settings::MapFixtureDoorLight;
	case ReadMode::Map_Fixture_Treasure:
		return Settings::MapFixtureTreasure;
	case ReadMode::Map_Fixture_Wall_Dark:
		return Settings::MapFixtureWallDark;
	case ReadMode::Map_Fixture_Wall_Light:
		return Settings::MapFixtureWallLight;
	case ReadMode::Map_Fixture_Menu:
		return Settings::MapFixtureMenu;
	}

	// Make sure we ALWAYS handle each mode's text.
	assert(false);

	return std::string();
}
