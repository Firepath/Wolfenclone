#pragma once

#include <unordered_map>

namespace HashtableExtension
{
	template <typename K, typename V, typename H>
	bool ContainsKey( const std::unordered_map<K,V,H>& hashtable, const K& key )
	{
		return hashtable.find( key ) != hashtable.end();
	}

	template <typename K, typename V, typename H>
	bool ContainsKey( const std::unique_ptr<std::unordered_map<K, V, H>>& hashtable, const K& key )
	{
		return hashtable->find( key ) != hashtable->end();
	}

	template <typename K, typename V, typename H>
	bool ContainsKey( const std::unordered_map<K, V, H>* const hashtable, const K& key )
	{
		return hashtable->find( key ) != hashtable->end();
	}
}