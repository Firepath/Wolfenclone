#pragma once

#include <fstream>
#include <string>

#include "Loader.h"
#include "StringKeyRepository.h"

class MapFixture;
class Surface;

class FixtureLoader : public Loader
{
public:
	FixtureLoader( StringKeyRepository<Surface>& surfaces );

	std::unique_ptr<StringKeyRepository<StringKeyRepository<MapFixture>>> GetFixtures();

private:
	void InitialiseAccess() override;
	void ReadLine( const std::string& line ) override;

	std::unique_ptr<StringKeyRepository<StringKeyRepository<MapFixture>>> Fixtures = nullptr;
	StringKeyRepository<Surface>& Surfaces;
};