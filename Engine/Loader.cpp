#include "Loader.h"

#include <algorithm>
#include <assert.h>

void Loader::Load( const std::string& filename )
{
	InitialiseLoad();
	_Load( filename );
	FinaliseLoad();
}

void Loader::InitialiseLoad()
{
}

void Loader::_Load( const std::string& filename )
{
	Mode = LoadConstants::ReadMode::None;

	std::string line;
	std::ifstream in( filename.c_str() );
	while ( std::getline( in, line ).good() )
	{
		if ( LoadConstants::UpdateReadMode( line, Mode ) )
		{
			continue;
		}

		ReadSetting( line );
	}

	Mode = LoadConstants::ReadMode::None;
}

void Loader::FinaliseLoad()
{
	// Ensure all settings are within acceptable bounds (which will also default any un-initialised settings).
	//StartingNumberOfTargets = std::min( MaxStartingNumberOfTargets, std::max( MinStartingNumberOfTargets, StartingNumberOfTargets ) );
}
