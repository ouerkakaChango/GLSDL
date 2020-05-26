#pragma once

#include "SDL.h"

#include <string>
#include <vector>
#include <map>
#include <array>
#include <list>
#include <functional>

#include "Numeric.h"

class Utility
{
public:
	Utility();
	~Utility();
};

void sure(bool b);
//////////////////////////////////////////////////////////////
//type
//把string转为其他类型
template <class T>
inline T Cast(const std::string& str);

//把string转为int类型
template <>
inline int Cast(const std::string& str);

//!!! 任意长度Create
//template<class T,class Arg>
//inline T* Create(Arg arg)
//{
//	return new T(arg);
//}
//
//template<class T, class Arg1,class Arg2>
//inline T* Create(Arg1 arg1, Arg2 arg2)
//{
//	return new T(arg1,arg2);
//}

typedef std::function<void()> Func;

//////////////////////////////////////////////////////////////
//string

typedef std::string Path;

bool IsExtention(const Path& filePath, const std::string& extention);

//////////////////////////////////////////////////////////////
//container

template <class T>
class MapVector
{
public:
	std::map<std::string, std::vector<T>> map_;
	inline bool Find(std::string key, std::vector<T>*& results);
};

template <class T>
class StringMap
{
public:
	std::map<std::string, T> map_;
	inline bool Find(std::string key, T*& result);
};

template <class T>
class Params
{
public:
	T operator[](unsigned i) { return datas_[i]; }
	std::vector<T> datas_;
};

template <class T1,class... T>
Params<T1> MakeParam(T1 start,T... var)
{
	Params<T1> re;
	re.datas_.push_back(start);
	return MakeParam(re, var...);
}

template <class T1, class... T>
Params<T1> MakeParam(Params<T1> re, T1 start, T... var)
{
	re.datas_.push_back(start);
	return MakeParam(re, var...);
}

template <class T1>
Params<T1> MakeParam(Params<T1> re)
{
	return re;
}

template <class T>
bool STL_Remove(std::vector<T>& vec,const T& elem);

//////////////////////////////////////////////////////////////
//math

bool InRect(int x, int y, SDL_Rect rect);

float Lerp(float min, float max, float k);

float FlashFunc(float k);

#include "Utility.inl"