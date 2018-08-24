#pragma once

#include <functional>
#include <string>
#include <unordered_map>

template <typename T>
class StringKeyRepository
{
public:
	void AddItem( const std::string key, std::unique_ptr<T> item )
	{
		Storage[key] = std::move( item );
	}

	T& GetItem( const std::string key )
	{
		return *(Storage.at( key ).get());
	}

private:
	std::unordered_map<std::string, std::unique_ptr<T>, std::hash<std::string>> Storage;
};
