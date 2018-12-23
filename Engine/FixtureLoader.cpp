#include "FileIOConstants.h"
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

void FixtureLoader::InitialiseAccess()
{
	Fixtures = std::make_unique<StringKeyRepository<StringKeyRepository<MapFixture>>>();
}

void FixtureLoader::ReadLine( const std::string & line )
{
	switch ( Mode )
	{
	case FileIOConstants::IOMode::Fixture_Door_Dark:
	case FileIOConstants::IOMode::Fixture_Door_Light:
	case FileIOConstants::IOMode::Fixture_Treasure:
	case FileIOConstants::IOMode::Fixture_Wall_Dark:
	case FileIOConstants::IOMode::Fixture_Wall_Light:
	{
		const size_t split = line.find( " " );
		const std::string name = line.substr( 0, split );
		std::string type = FileIOConstants::GetIOModeText( Mode );
		const std::string textureName = line.substr( split + 1, line.length() - split );

		const Surface* texture = &(Surfaces.GetItem( textureName ));
		std::unique_ptr<MapFixture> fixture = nullptr;

		switch ( Mode )
		{
		case FileIOConstants::IOMode::Fixture_Door_Dark:
		case FileIOConstants::IOMode::Fixture_Door_Light:
			// TODO: Door type
		case FileIOConstants::IOMode::Fixture_Wall_Dark:
		case FileIOConstants::IOMode::Fixture_Wall_Light:
			fixture = std::make_unique<Wall>( name, type, texture );
			break;
		case FileIOConstants::IOMode::Fixture_Treasure:
			fixture = std::make_unique<Treasure>( name, type, texture );
			break;
		default:
			break;
		}

		if ( fixture != nullptr )
		{
			const std::string fixtureType = FileIOConstants::GetIOModeText( Mode );
			if ( !Fixtures->Contains( fixtureType ) )
			{
				Fixtures->AddItem( fixtureType, std::make_unique<StringKeyRepository<MapFixture>>() );
			}

			StringKeyRepository<MapFixture>& fixtures = Fixtures->GetItem( fixtureType );
			fixtures.AddItem( name, std::move( fixture ) );
		}
		break;
	}
	}
}
