#pragma once

#include "Utility.h"

class Action;
class Effect;

class Timeline
{
public:
	void AddAction(float time,Action* action);
	void AddAction(float time, Func function);
	void AddEffect(float time, Effect* effect);
	void Reset();
	void Update(float deltaTime);
	float Now() { return now_; }
protected:
	std::vector<Action*> actions_;	//Ë²Ê±
	std::vector<Effect*> effects_;	//³ÖÐø
	float now_{0.0f};
};