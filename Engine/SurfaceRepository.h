#pragma once

#include <functional>
#include <string>
#include <unordered_map>

class Surface;

class SurfaceRepository
{
public:
	void AddSurface( const std::string name, std::unique_ptr<Surface> surface );
	Surface& GetSurface( const std::string name );

private:
	std::unordered_map<std::string, std::unique_ptr<Surface>, std::hash<std::string>> Surfaces;
};