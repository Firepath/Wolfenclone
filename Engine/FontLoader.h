#pragma once

#include <fstream>
#include <string>

#include "LoadConstants.h"
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
	void InitialiseLoad() override;
	void ReadSetting( const std::string& line );

	std::unique_ptr<StringKeyRepository<Font>> Fonts = nullptr;
	StringKeyRepository<Surface>& Surfaces;
};