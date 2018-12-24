#include "FixtureLoader.h"

#include "Ammo.h"
#include "Door.h"
#include "FileIOConstants.h"
#include "Furniture.h"
#include "Health.h"
#include "Key.h"
#include "Treasure.h"
#include "Wall.h"
#include "Weapon.h"

FixtureLoader::FixtureLoader( StringKeyRepository<Surface>& surfaces )
	:
	Surfaces( surfaces )
{
}

std::unique_ptr<StringKeyRepository<StringKeyRepository<Fixture>>> FixtureLoader::GetFixtures()
{
	return std::move( Fixtures );
}

void FixtureLoader::InitialiseAccess()
{
	Fixtures = std::make_unique<StringKeyRepository<StringKeyRepository<Fixture>>>();
}

void FixtureLoader::ReadLine( const std::string & line )
{
	switch ( Mode )
	{
	case FileIOConstants::IOMode::Fixture_Ammo:
	case FileIOConstants::IOMode::Fixture_Decoration:
	case FileIOConstants::IOMode::Fixture_Door_Dark:
	case FileIOConstants::IOMode::Fixture_Door_Light:
	case FileIOConstants::IOMode::Fixture_Furniture:
	case FileIOConstants::IOMode::Fixture_Health:
	case FileIOConstants::IOMode::Fixture_Key:
	case FileIOConstants::IOMode::Fixture_Treasure:
	case FileIOConstants::IOMode::Fixture_Wall_Dark:
	case FileIOConstants::IOMode::Fixture_Wall_Light:
	case FileIOConstants::IOMode::Fixture_Weapon:
	{
		const size_t split = line.find( " " );
		const std::string name = line.substr( 0, split );
		std::string type = FileIOConstants::GetIOModeText( Mode );
		const std::string textureName = line.substr( split + 1, line.length() - split );

		const Surface* texture = &(Surfaces.GetItem( textureName ));
		std::unique_ptr<Fixture> fixture = nullptr;

		switch ( Mode )
		{
		case FileIOConstants::IOMode::Fixture_Ammo:
			fixture = std::make_unique<Ammo>( name, type, texture );
			break;
		case FileIOConstants::IOMode::Fixture_Decoration:
			fixture = std::make_unique<Decoration>( name, type, texture );
			break;
		case FileIOConstants::IOMode::Fixture_Door_Dark:
		case FileIOConstants::IOMode::Fixture_Door_Light:
			fixture = std::make_unique<Door>( name, type, texture );
			break;
		case FileIOConstants::IOMode::Fixture_Furniture:
			fixture = std::make_unique<Furniture>( name, type, texture );
			break;
		case FileIOConstants::IOMode::Fixture_Health:
			fixture = std::make_unique<Health>( name, type, texture );
			break;
		case FileIOConstants::IOMode::Fixture_Key:
			fixture = std::make_unique<Key>( name, type, texture );
			break;
		case FileIOConstants::IOMode::Fixture_Treasure:
			fixture = std::make_unique<Treasure>( name, type, texture );
			break;
		case FileIOConstants::IOMode::Fixture_Wall_Dark:
		case FileIOConstants::IOMode::Fixture_Wall_Light:
			fixture = std::make_unique<Wall>( name, type, texture );
			break;
		case FileIOConstants::IOMode::Fixture_Weapon:
			fixture = std::make_unique<Weapon>( name, type, texture );
			break;
		default:
			break;
		}

		if ( fixture != nullptr )
		{
			const std::string fixtureType = FileIOConstants::GetIOModeText( Mode );
			if ( !Fixtures->Contains( fixtureType ) )
			{
				Fixtures->AddItem( fixtureType, std::make_unique<StringKeyRepository<Fixture>>() );
			}

			StringKeyRepository<Fixture>& fixtures = Fixtures->GetItem( fixtureType );
			fixtures.AddItem( name, std::move( fixture ) );
		}
		break;
	}
	}
}
