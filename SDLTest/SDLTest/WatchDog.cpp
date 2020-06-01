#include "WatchDog.h"


WatchDog::WatchDog()
{
}


WatchDog::~WatchDog()
{
}

float WatchDog::ToFloat(Uint32 dur)
{
	return (float)dur / 1000;
}

float WatchDog::Lapse()
{
	now_ = SDL_GetTicks();
	return ((float)(now_ - last_)) / 1000;
}

void WatchDog::Tick()
{
	last_ = now_;
}

void WatchDog::StartWatch()
{
	watchList_.clear();
	watchLast_ = SDL_GetTicks();
}

void WatchDog::Watch(const std::string& tag)
{
	watchNow_ = SDL_GetTicks();
	watchList_[tag] = ToFloat(watchNow_ - watchLast_);
	watchLast_ = watchNow_;
}

void WatchDog::Record()
{
	WatchRecord re;
	re.watchList_ = watchList_;
	recordList_.push_back(re);
}