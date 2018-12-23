#pragma once

#include <fstream>
#include <string>

#include "Loader.h"
#include "StringKeyRepository.h"

class Surface;

class TextureLoader : public Loader
{
public:
	std::unique_ptr<StringKeyRepository<Surface>> GetTextures();

private:
	void InitialiseAccess() override;
	void ReadLine( const std::string& line ) override;

	std::unique_ptr<StringKeyRepository<Surface>> Surfaces = nullptr;
};