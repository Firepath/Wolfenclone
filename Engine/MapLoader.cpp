#include "FileIOConstants.h"
#include "MapLoader.h"
#include "Map.h"
#include "Vec2.h"

MapLoader::MapLoader( StringKeyRepository<StringKeyRepository<Fixture>>& fixtures )
	:
	Fixtures( fixtures )
{
}

std::unique_ptr<Map> MapLoader::GetMap()
{
	return std::move( _Map );
}

void MapLoader::InitialiseAccess()
{
	_Map = std::make_unique<Map>();
}

void MapLoader::ReadLine( const std::string& line )
{
	switch ( Mode )
	{
	case FileIOConstants::IOMode::Map_Name:
	{
		_Map->SetName( line );
		break;
	}
	case FileIOConstants::IOMode::Map_Cell_Content:
	{
		const size_t split = line.find( " " );
		const std::string cellLocation = line.substr( 0, split );
		std::string fixture = line.substr( split + 1, line.length() - split );

		const std::string fixtureList = FindFirstInLineTextAndRemoveAndReturn( fixture, '[', ']', true );

		const size_t xySplit = cellLocation.find( "," );
		const std::string xString = cellLocation.substr( 0, xySplit );
		const std::string yString = cellLocation.substr( xySplit + 1, cellLocation.length() - xySplit );

		Vei2 location( std::stoi( xString ), std::stoi( yString ) );

		_Map->GetGrid().Fill( location, &Fixtures.GetItem( fixtureList ).GetItem( fixture ) );
		break;
	}
	}
}
