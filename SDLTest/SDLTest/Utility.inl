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