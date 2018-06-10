#pragma once

#include <vector>

namespace VectorExtension
{
	template <typename T>
	bool Contains( const std::vector<T>& vec, const T& val )
	{
		return std::find( vec.begin(), vec.end(), val ) != vec.end();
	}

	template <typename T>
	bool Contains( const std::unique_ptr<std::vector<T>>& vec, const T& val )
	{
		return std::find( vec->begin(), vec->end(), val ) != vec->end();
	}
}