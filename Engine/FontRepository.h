#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "Font.h"

class FontRepository
{
public:
	void AddFont( const std::string name, std::unique_ptr<Font> font );
	Font& GetFont( const std::string name );

private:
	std::unordered_map<std::string, std::unique_ptr<Font>, std::hash<std::string>> Fonts;
};