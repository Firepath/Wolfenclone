#pragma once

#include "Decoration.h"

class Furniture : public Decoration
{
public:
	Furniture( const std::string name, const std::string type, const Surface* const texture );

	virtual const bool BlocksMovement() const override;
};