#pragma once

#include <fstream>
#include <string>

//#include "Graphics.h"
//#include "Vec2I.h"

class Settings
{
public:
	Settings();
	void LoadSettings( std::string filename );

private:
	static constexpr char StartingNumberOfTargetsSetting[] = "[Starting Number of Targets]";
	static constexpr unsigned int MinStartingNumberOfTargets = 3;
	static constexpr unsigned int MaxStartingNumberOfTargets = 10;

	void FinaliseSettings();

	unsigned int StartingNumberOfTargets;
};