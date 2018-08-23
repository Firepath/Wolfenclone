#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

class Settings
{
public:
	enum class ReadMode
	{
		None,
		List_Files,
		Texture_Wall_Dark,
		Texture_Wall_Light,
		EnumOptionsCount // This is the size of the enum, don't add things after it
	};

	static constexpr char None[] = "[None]";
	static constexpr char ListFiles[] = "[List Files]";
	static constexpr char TextureWallDark[] = "[Texture Wall Dark]";
	static constexpr char TextureWallLight[] = "[Texture Wall Light]";

	Settings();

	const std::unordered_map<std::string, std::string, std::hash<std::string>>& GetSettingList( ReadMode mode );
	void LoadSettings( const std::string& filename );

private:
	void FinaliseSettings();
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
};