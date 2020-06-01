#pragma once

#include "Utility.h"

class Action;
class Effect;
class WatchDog;

class Timeline
{
public:
	void AddAction(float time,Action* action);
	void AddAction(float time, Func function);
	void AddEffect(float time, Effect* effect);
	void Reset();
	void Update(float deltaTime);
	void Update(float deltaTime, WatchDog& watchDog);
	float Now() { return now_; }
protected:
	std::vector<Action*> actions_;	//˲ʱ
	std::vector<Effect*> effects_;	//����
	float now_;
};