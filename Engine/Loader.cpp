#include "FileIOConstants.h"
#include "Loader.h"

#include <algorithm>
#include <assert.h>

void Loader::Load( const std::string& filename )
{
	AccessFile( filename );
}

std::string Loader::FindFirstInLineTextAndRemoveAndReturn( std::string& line, const char startDelimiter, const char endDelimiter, bool includeDelimiters )
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
		end -= 1;
	}

	const size_t length = end - (start - 1);

	std::string text = line.substr( start, length );

	size_t lineStart = end + 1;
	if ( !includeDelimiters )
	{
		lineStart += 1;
	}

	size_t lineLength = line.size() - lineStart;

	line = line.substr( lineStart, lineLength );

	if ( line.find( " " ) == 0 )
	{
		line = line.substr( 1, line.length() - 1 );
	}

	return text;
}

void Loader::FinaliseAccess()
{
	// Ensure all settings are within acceptable bounds (which will also default any un-initialised settings).
	//StartingNumberOfTargets = std::min( MaxStartingNumberOfTargets, std::max( MinStartingNumberOfTargets, StartingNumberOfTargets ) );
}

void Loader::InitialiseAccess()
{
}

void Loader::ProcessFile( const std::string& filename )
{
	Mode = FileIOConstants::IOMode::None;

	std::string line;
	std::ifstream in( filename );
	if ( !in.good() )
	{
		// TODO: More than just return - let someone know.
		return;
	}

	while ( in.good() )
	{
		std::getline( in, line );
		if ( UpdateIOMode( line ) )
		{
			continue;
		}

		ReadLine( line );
	}

	Mode = FileIOConstants::IOMode::None;
}
