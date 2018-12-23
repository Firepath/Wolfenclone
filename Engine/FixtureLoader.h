#pragma once

#include <fstream>
#include <string>

#include "Loader.h"
#include "StringKeyRepository.h"

class Fixture;
class Surface;

class FixtureLoader : public Loader
{
public:
	FixtureLoader( StringKeyRepository<Surface>& surfaces );

	std::unique_ptr<StringKeyRepository<StringKeyRepository<Fixture>>> GetFixtures();

private:
	void InitialiseAccess() override;
	void ReadLine( const std::string& line ) override;

	std::unique_ptr<StringKeyRepository<StringKeyRepository<Fixture>>> Fixtures = nullptr;
	StringKeyRepository<Surface>& Surfaces;
};