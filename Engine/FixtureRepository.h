#pragma once

#include <functional>
#include <string>
#include <unordered_map>

class MapFixture;

class FixtureRepository
{
public:
	void AddFixture( const std::string name, std::unique_ptr<MapFixture> fixture );
	MapFixture& GetFixture( const std::string name );

private:
	std::unordered_map<std::string, std::unique_ptr<MapFixture>, std::hash<std::string>> Fixtures;
};