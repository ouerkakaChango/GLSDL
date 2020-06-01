#pragma once
//based on SDL
//watch informations for the game
//such as performance data...
//--Chango
#include "Utility.h"

struct WatchRecord
{
	std::map<std::string, float> watchList_;
};

class WatchDog
{
public:
	WatchDog();
	~WatchDog();
	float Lapse();
	void Tick();
	void StartWatch();
	void Watch(const std::string& tag);
	void Record();
	std::map<std::string, float> GetWatchList() const{
		return watchList_
			;
	};
protected:
	float ToFloat(Uint32 dur);
	Uint32 now_, last_{0};
	Uint32 watchNow_, watchLast_{0};
	std::map<std::string, float> watchList_;
	std::vector<WatchRecord> recordList_;
};

