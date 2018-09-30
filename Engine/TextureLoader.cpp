#include "TextureLoader.h"
#include "Surface.h"

std::unique_ptr<StringKeyRepository<Surface>> TextureLoader::GetTextures()
{
	return std::move( Surfaces );
}

void TextureLoader::InitialiseLoad()
{
	Surfaces = std::make_unique<StringKeyRepository<Surface>>();
}

void TextureLoader::ReadSetting( const std::string& line )
{
	switch ( Mode )
	{
	case LoadConstants::ReadMode::Texture_Door_Dark:
	case LoadConstants::ReadMode::Texture_Door_Light:
	case LoadConstants::ReadMode::Texture_Font:
	case LoadConstants::ReadMode::Texture_Treasure:
	case LoadConstants::ReadMode::Texture_Wall_Dark:
	case LoadConstants::ReadMode::Texture_Wall_Light:
	{
		const size_t split = line.find( " " );
		const std::string name = line.substr( 0, split );
		const std::string filename = line.substr( split + 1, line.length() - split );

		Surfaces->AddItem( name, std::make_unique<Surface>( filename ) );
	}
	}
}
