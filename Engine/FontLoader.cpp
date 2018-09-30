#include "FontLoader.h"
#include "Font.h"

FontLoader::FontLoader( StringKeyRepository<Surface>& surfaces )
	:
	Surfaces( surfaces )
{
}
std::unique_ptr<StringKeyRepository<Font>> FontLoader::GetFonts()
{
	return std::move( Fonts );
}

void FontLoader::InitialiseLoad()
{
	Fonts = std::make_unique<StringKeyRepository<Font>>();
}

void FontLoader::ReadSetting( const std::string & line )
{
	switch ( Mode )
	{
	case LoadConstants::ReadMode::Font:
	{
		const size_t split = line.find( " " );
		const std::string name = line.substr( 0, split );
		const std::string textureName = line.substr( split + 1, line.length() - split );

		// TODO: Add colour, size, and alpha colour to config file to be read in
		Fonts->AddItem( name, std::make_unique<Font>( Surfaces.GetItem( textureName ), Colors::White, 14, Colors::White ) );
	}
	}
}
