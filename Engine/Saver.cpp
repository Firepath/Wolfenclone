#include "Saver.h"

#include <algorithm>
#include <assert.h>

void Saver::Save( const std::string& filename )
{
	AccessFile( filename );
}

void Saver::WriteIOModeTag( std::ofstream& out )
{
	const std::string modeLine = FileIOConstants::GetIOModeText( Mode );
	out << modeLine << std::endl;
	const std::string commentLine = FileIOConstants::GetIOModeCommentText( Mode );
	out << commentLine << std::endl;
}

void Saver::FinaliseAccess()
{
}

void Saver::InitialiseAccess()
{
}
