#pragma once

#include <fstream>
#include <string>

#include "LoadConstants.h"
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
	void InitialiseLoad() override;
	void ReadSetting( const std::string& line );

	std::unique_ptr<StringKeyRepository<StringKeyRepository<MapFixture>>> Fixtures = nullptr;
	StringKeyRepository<Surface>& Surfaces;
};