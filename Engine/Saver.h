#pragma once

#include <fstream>
#include <string>

#include "FileAccesser.h"
#include "FileIOConstants.h"

class Saver : public FileAccesser
{
public:
	virtual void Save( const std::string& filename );

protected:
	virtual void WriteIOModeTag( std::ofstream& out );

private:
	virtual void FinaliseAccess() override;
	virtual void InitialiseAccess() override;
	virtual void ProcessFile( const std::string& filename ) override = 0;
	virtual void WriteLine( const std::string& line, std::ofstream& out ) = 0;
};
