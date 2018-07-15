#include "SurfaceRepository.h"

void SurfaceRepository::AddSurface( const std::string name, std::unique_ptr<Surface> surface )
{
	Surfaces[name] = std::move( surface );
}

Surface& SurfaceRepository::GetSurface( const std::string name )
{
	return *(Surfaces.at( name ).get());
}
