#pragma once

#include "Utility.h"

class Action;

class Timeline
{
public:
	void AddAction(float time,Action* action);
	void Start();
	void Update(float deltaTime);
protected:
	std::vector<Action*> actions_;
	float now_;
};