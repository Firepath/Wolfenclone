#pragma once

#include <fstream>
#include <string>

#include "LoadConstants.h"

class Loader
{
public:
	virtual void Load( const std::string& filename );

protected:
	LoadConstants::ReadMode Mode = LoadConstants::ReadMode::None;

	virtual void InitialiseLoad();
	virtual void ReadSetting( const std::string& line ) = 0;

private:
	void _Load( const std::string& filename );
	virtual void FinaliseLoad();
};
