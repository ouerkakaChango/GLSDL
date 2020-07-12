#pragma once
#include <string>

template <class T>
T Cast(const std::string& str)
{
	T t;
	return t;
}

template <>
int Cast(const std::string& str)
{
	return std::stoi(str);
}

template <class T>
bool MapVector<T>::Find(std::string key, std::vector<T>*& results)
{
	auto iter = map_.find(key);
	if (iter != map_.end())
	{
		results = &iter->second;
		return true;
	}
	else
	{
		return false;
	}
}

template <class T>
bool MapVector<T>::HasKey(const std::string name)
{
	return map_.find(name) != map_.end();
}

template<class T>
bool StringMap<T>::Find(std::string key, T*& result)
{
	auto iter = map_.find(key);
	if (iter != map_.end())
	{
		result = &iter->second;
		return true;
	}
	else
	{
		return false;
	}
}

template <class T>
bool STL_Remove(std::vector<T>& vec, const T& elem)
{
	for (auto iter = vec.begin(); iter != vec.end(); iter++)
	{
		if (*iter == elem)
		{
			vec.erase(iter);
			return true;
		}
	}
	return false;
}

template <class T>
bool STL_RemoveAll(std::vector<T>& vec, const T& elem)
{
	vec.erase(std::remove(vec.begin(), vec.end(), elem), vec.end());
}

template <class Key, class val>
val* STLMapGet(std::map<Key, val*> map, const Key& key)
{
	auto iter = map.find(key);
	bool found = iter != map.end();
	val* re = nullptr;
	if (found && iter->first == key) { re = iter->second; }
	return re;
}