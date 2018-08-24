#include "MapFixture.h"
#include "FixtureRepository.h"

void FixtureRepository::AddFixture( const std::string name, std::unique_ptr<MapFixture> surface )
{
	Fixtures[name] = std::move( surface );
}

MapFixture& FixtureRepository::GetFixture( const std::string name )
{
	return *(Fixtures.at( name ).get());
}
