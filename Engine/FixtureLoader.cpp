#include "FixtureLoader.h"
#include "Treasure.h"
#include "Wall.h"

FixtureLoader::FixtureLoader( StringKeyRepository<Surface>& surfaces )
	:
	Surfaces( surfaces )
{
}

std::unique_ptr<StringKeyRepository<StringKeyRepository<MapFixture>>> FixtureLoader::GetFixtures()
{
	return std::move( Fixtures );
}

void FixtureLoader::InitialiseLoad()
{
	Fixtures = std::make_unique<StringKeyRepository<StringKeyRepository<MapFixture>>>();
}

void FixtureLoader::ReadSetting( const std::string & line )
{
	switch ( Mode )
	{
	case LoadConstants::ReadMode::Fixture_Door_Dark:
	case LoadConstants::ReadMode::Fixture_Door_Light:
	case LoadConstants::ReadMode::Fixture_Treasure:
	case LoadConstants::ReadMode::Fixture_Wall_Dark:
	case LoadConstants::ReadMode::Fixture_Wall_Light:
	{
		const size_t split = line.find( " " );
		const std::string name = line.substr( 0, split );
		const std::string textureName = line.substr( split + 1, line.length() - split );

		const Surface* texture = &(Surfaces.GetItem( textureName ));
		std::unique_ptr<MapFixture> fixture = nullptr;

		switch ( Mode )
		{
		case LoadConstants::ReadMode::Fixture_Door_Dark:
		case LoadConstants::ReadMode::Fixture_Door_Light:
		case LoadConstants::ReadMode::Fixture_Wall_Dark:
		case LoadConstants::ReadMode::Fixture_Wall_Light:
			fixture = std::make_unique<Wall>( texture );
			break;
		case LoadConstants::ReadMode::Fixture_Treasure:
			fixture = std::make_unique<Treasure>( texture );
			break;
		default:
			break;
		}

		if ( fixture != nullptr )
		{
			const std::string fixtureType = LoadConstants::GetReadModeText( Mode );
			if ( !Fixtures->Contains( fixtureType ) )
			{
				Fixtures->AddItem( fixtureType, std::make_unique<StringKeyRepository<MapFixture>>() );
			}

			StringKeyRepository<MapFixture>& fixtures = Fixtures->GetItem( fixtureType );
			fixtures.AddItem( name, std::move( fixture ) );
		}
	}
	}
}
