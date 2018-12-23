#pragma once

#include <fstream>
#include <string>

#include "Loader.h"
#include "StringKeyRepository.h"

class Font;
class Surface;

class FontLoader : public Loader
{
public:
	FontLoader( StringKeyRepository<Surface>& surfaces );

	std::unique_ptr<StringKeyRepository<Font>> GetFonts();

private:
	void InitialiseAccess() override;
	void ReadLine( const std::string& line ) override;

	std::unique_ptr<StringKeyRepository<Font>> Fonts = nullptr;
	StringKeyRepository<Surface>& Surfaces;
};