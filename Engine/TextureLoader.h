#pragma once

#include <fstream>
#include <string>

#include "LoadConstants.h"
#include "Loader.h"
#include "StringKeyRepository.h"

class Surface;

class TextureLoader : public Loader
{
public:
	std::unique_ptr<StringKeyRepository<Surface>> GetTextures();

private:
	void InitialiseLoad() override;
	void ReadSetting( const std::string& line ) override;

	std::unique_ptr<StringKeyRepository<Surface>> Surfaces = nullptr;
};