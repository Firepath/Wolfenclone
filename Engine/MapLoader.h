#pragma once

#include <fstream>
#include <string>

#include "Loader.h"
#include "StringKeyRepository.h"

class Fixture;
class Map;

class MapLoader : public Loader
{
public:
	MapLoader( StringKeyRepository<StringKeyRepository<Fixture>>& fixtures );

	std::unique_ptr<Map> GetMap();

private:
	void InitialiseAccess() override;
	void ReadLine( const std::string& line ) override;

	std::unique_ptr<Map> _Map = nullptr;
	StringKeyRepository<StringKeyRepository<Fixture>>& Fixtures;
};