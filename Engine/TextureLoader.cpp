#include "FileIOConstants.h"
#include "TextureLoader.h"
#include "Surface.h"

std::unique_ptr<StringKeyRepository<Surface>> TextureLoader::GetTextures()
{
	return std::move( Surfaces );
}

void TextureLoader::InitialiseAccess()
{
	Surfaces = std::make_unique<StringKeyRepository<Surface>>();
}

void TextureLoader::ReadLine( const std::string& line )
{
	switch ( Mode )
	{
	case FileIOConstants::IOMode::Texture_Ammo:
	case FileIOConstants::IOMode::Texture_Decoration:
	case FileIOConstants::IOMode::Texture_Door_Dark:
	case FileIOConstants::IOMode::Texture_Door_Light:
	case FileIOConstants::IOMode::Texture_Font:
	case FileIOConstants::IOMode::Texture_Furniture:
	case FileIOConstants::IOMode::Texture_Health:
	case FileIOConstants::IOMode::Texture_Key:
	case FileIOConstants::IOMode::Texture_Treasure:
	case FileIOConstants::IOMode::Texture_Wall_Dark:
	case FileIOConstants::IOMode::Texture_Wall_Light:
	case FileIOConstants::IOMode::Texture_Weapon:
	{
		const size_t split = line.find( " " );
		const std::string name = line.substr( 0, split );
		const std::string filename = line.substr( split + 1, line.length() - split );

		Surfaces->AddItem( name, std::make_unique<Surface>( filename ) );
		break;
	}
	}
}
