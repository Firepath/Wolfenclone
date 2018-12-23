#pragma once

#include <fstream>
#include <string>

#include "FileAccesser.h"
#include "FileIOConstants.h"

class Loader : public FileAccesser
{
public:
	virtual void Load( const std::string& filename );

protected:
	std::string FindFirstInLineTextAndRemoveAndReturn( std::string& line, const char startDelimiter, const char endDelimiter, bool includeDelimiters = false );

private:
	virtual void FinaliseAccess() override;
	virtual void InitialiseAccess() override;
	virtual void ProcessFile( const std::string& filename ) override;
	virtual void ReadLine( const std::string& line ) = 0;
};
