#pragma once

#include "Utility.h"

class Action;

class Timeline
{
public:
	void AddAction(float time,Action* action);
	void AddAction(float time, Func function);
	void Reset();
	void Update(float deltaTime);
	float Now() { return now_; }
protected:
	std::vector<Action*> actions_;
	float now_;
};