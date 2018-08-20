#include "FontRepository.h"

void FontRepository::AddFont( const std::string name, std::unique_ptr<Font> font )
{
	Fonts[name] = std::move( font );
}

Font& FontRepository::GetFont( const std::string name )
{
	return *(Fonts.at( name ).get());
}
