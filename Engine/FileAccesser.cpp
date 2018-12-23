#include "FileAccesser.h"

void FileAccesser::AccessFile( const std::string & filename )
{
	InitialiseAccess();
	ProcessFile( filename );
	FinaliseAccess();
}

bool FileAccesser::UpdateIOMode( std::string line )
{
	// If a blank line or a comment line, skip it.
	if ( line.length() == 0 || line.find( FileIOConstants::Comment, 0 ) == 0 )
	{
		return true;
	}

	bool setMode = false;
	for ( int mode = (int)FileIOConstants::IOMode::None; mode != (int)FileIOConstants::IOMode::EnumOptionsCount; mode++ )
	{
		if ( line.find( FileIOConstants::GetIOModeText( (FileIOConstants::IOMode)mode ), 0 ) == 0 )
		{
			Mode = (FileIOConstants::IOMode)mode;
			setMode = true;
			break;
		}
	}

	return setMode;
}
