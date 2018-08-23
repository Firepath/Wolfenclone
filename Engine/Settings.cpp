#include <algorithm>
#include <assert.h>

#include "Settings.h"

Settings::Settings()
{
	SettingsLists[Settings::ListFiles] = std::make_unique<std::unordered_map<std::string, std::string, std::hash<std::string>>>();
	SettingsLists[Settings::TextureWallDark] = std::make_unique<std::unordered_map<std::string, std::string, std::hash<std::string>>>();
	SettingsLists[Settings::TextureWallLight] = std::make_unique<std::unordered_map<std::string, std::string, std::hash<std::string>>>();
}

const std::unordered_map<std::string, std::string, std::hash<std::string>>& Settings::GetSettingList( ReadMode mode )
{
	return *(SettingsLists.at( GetReadModeText( mode ) ).get());
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

void Settings::_LoadSettings( const std::string& filename )
{
	std::string line;
	std::ifstream in( filename.c_str() );

	while ( std::getline( in, line ).good() )
	{
		if ( line.length() == 0 )
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
	case ReadMode::Texture_Wall_Dark:
	case ReadMode::Texture_Wall_Light:
		size_t split = line.find( " " );
		std::string name = line.substr( 0, split );
		std::string filename = line.substr( split + 1, line.length() - split );

		auto& list = *(SettingsLists.at( GetReadModeText( Mode ) ).get());
		list[name] = filename;
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
	case ReadMode::Texture_Wall_Dark:
		return Settings::TextureWallDark;
	case ReadMode::Texture_Wall_Light:
		return Settings::TextureWallLight;
	}

	// Make sure we ALWAYS handle each mode's text.
	assert(false);

	return std::string();
}
