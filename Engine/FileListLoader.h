#pragma once

#include <string>
#include <unordered_map>

#include "LoadConstants.h"
#include "Loader.h"

class FileListLoader : public Loader
{
public:
	const std::unordered_map<std::string, std::string, std::hash<std::string>>& GetFileList() const;

private:
	void ReadSetting( const std::string& line ) override;

	std::unordered_map<std::string, std::string, std::hash<std::string>> FileList;
};