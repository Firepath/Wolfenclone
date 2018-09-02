#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

class MenuStructure
{
public:
	MenuStructure( const std::string name, std::string parent, const size_t columns = 1 )
		:
		Name( name ),
		Parent( parent ),
		Columns( columns )
	{
	}

	const std::string Name;
	const std::string Parent;
	std::vector<std::string> Items;
	size_t Columns;
};

inline bool operator==( const MenuStructure lhs, const std::string rhs )
{
	return lhs.Name == rhs;
}

class Settings
{
public:
	enum class ReadMode
	{
		None,
		List_Files,
		Texture_Wall_Dark,
		Texture_Wall_Light,
		Map_Fixture_Wall_Dark,
		Map_Fixture_Wall_Light,
		Map_Fixture_Menu,
		EnumOptionsCount // This is the size of the enum, don't add things after it
	};

	static constexpr char Comment[] = "'";

	static constexpr char None[] = "[None]";
	static constexpr char ListFiles[] = "[List Files]";
	static constexpr char TextureWallDark[] = "[Texture Wall Dark]";
	static constexpr char TextureWallLight[] = "[Texture Wall Light]";
	static constexpr char MapFixtureWallDark[] = "[Fixture Wall Dark]";
	static constexpr char MapFixtureWallLight[] = "[Fixture Wall Light]";
	static constexpr char MapFixtureMenu[] = "[Fixture Menu]";

	Settings();

	const std::unordered_map<std::string, std::string, std::hash<std::string>>& GetSettingList( ReadMode mode );
	const std::vector<MenuStructure>& GetFixtureMenuStructure();
	void LoadSettings( const std::string& filename );

private:
	void FinaliseSettings();
	std::string FindFirstInLineTextAndRemoveAndReturn( std::string& line, const char startDelimiter, const char endDelimiter, bool includeDelimiters = false );
	void _LoadSettings( const std::string& filename );
	void ReadSetting( const std::string& line );
	std::string GetReadModeText( const ReadMode mode ) const;

private:
	//static constexpr char StartingNumberOfTargetsSetting[] = "[Starting Number of Targets]";
	//static constexpr unsigned int MinStartingNumberOfTargets = 3;
	//static constexpr unsigned int MaxStartingNumberOfTargets = 10;

	//unsigned int StartingNumberOfTargets;

	ReadMode Mode = ReadMode::None;
	std::unordered_map<std::string, std::unique_ptr<std::unordered_map<std::string, std::string, std::hash<std::string>>>, std::hash<std::string>> SettingsLists;
	std::vector<MenuStructure> FixtureMenuStructure;
};
