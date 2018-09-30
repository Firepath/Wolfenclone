#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

template <typename T>
class StringKeyRepository
{
public:
	void AddItem( const std::string key, std::unique_ptr<T> item )
	{
		Storage[key] = std::move( item );
	}

	const bool Contains( const std::string key ) const
	{
		return Storage.find( key ) != Storage.end();
	}

	T& GetItem( const std::string key ) const
	{
		return *(Storage.at( key ).get());
	}

	const std::vector<std::string> GetKeys() const
	{
		std::vector<std::string> keys;
		keys.reserve( Storage.size() );
		for ( auto& it : Storage )
		{
			keys.push_back( it.first );
		}

		return keys;
	}

	const size_t GetSize() const
	{
		return Storage.size();
	}

private:
	std::unordered_map<std::string, std::unique_ptr<T>, std::hash<std::string>> Storage;
};
