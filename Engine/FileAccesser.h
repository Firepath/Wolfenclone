#pragma once

#include <fstream>
#include <string>

#include "FileIOConstants.h"

class FileAccesser
{
protected:
	FileIOConstants::IOMode Mode = FileIOConstants::IOMode::None;

	virtual void AccessFile( const std::string& filename );
	bool UpdateIOMode( std::string line );

private:
	virtual void FinaliseAccess() = 0;
	virtual void InitialiseAccess() = 0;
	virtual void ProcessFile( const std::string& filename ) = 0;
};
