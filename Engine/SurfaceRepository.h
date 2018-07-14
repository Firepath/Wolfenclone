#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "Surface.h"

class SurfaceRepository
{
public:
	void AddSurface( const std::string name, const Surface& surface );
	Surface* GetSurface( const std::string name );

private:
	std::unordered_map<std::string, Surface, std::hash<std::string>> Surfaces;
};