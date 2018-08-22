#include <algorithm>

#include "Settings.h"

Settings::Settings()
{
}

void Settings::LoadSettings( std::string filename )
{
	std::string line;
	std::ifstream in( filename.c_str() );

	while ( std::getline( in, line ).good() )
	{
		if ( line.find( Settings::StartingNumberOfTargetsSetting, 0 ) == 0 )
		{
			in >> StartingNumberOfTargets;
		}
	}

	FinaliseSettings();
}

void Settings::FinaliseSettings()
{
	// Ensure all settings are within acceptable bounds (which will also default any un-initialised settings).
	StartingNumberOfTargets = std::min( MaxStartingNumberOfTargets, std::max( MinStartingNumberOfTargets, StartingNumberOfTargets ) );
}
