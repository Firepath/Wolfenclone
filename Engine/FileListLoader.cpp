#include "FileIOConstants.h"
#include "FileListLoader.h"

const std::unordered_map<std::string, std::string, std::hash<std::string>>& FileListLoader::GetFileList() const
{
	return FileList;
}

void FileListLoader::ReadLine( const std::string& line )
{
	switch ( Mode )
	{
	case FileIOConstants::IOMode::List_Files:
	{
		size_t split = line.find( " " );
		std::string name = line.substr( 0, split );
		std::string filename = line.substr( split + 1, line.length() - split );

		FileList[name] = filename;
		break;
	}
	}
}
