#include "SurfaceRepository.h"

void SurfaceRepository::AddSurface( const std::string name, const Surface& surface )
{
	Surfaces[name] = surface;
}

Surface* SurfaceRepository::GetSurface( const std::string name )
{
	return &Surfaces.at( name );
}
