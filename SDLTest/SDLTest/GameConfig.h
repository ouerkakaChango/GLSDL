#pragma once
#include <map>

#include "Utility.h"

using std::string;

class GameConfig
{
public:
	GameConfig();
	~GameConfig();

	bool Load(const string& filePath="");
	string operator[](const string& key);
	template<class T>
	T Get(const string& key) { return Cast<T>(content_[key]); }
private:
	bool LoadDefaultConfig();
	std::map<string, string> content_;
};

