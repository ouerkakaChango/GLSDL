#pragma once
#include "Object.h"

#include "Utility.h"

class Drawable;
class Condition : public Object
{
public:
	Condition();
	~Condition();

	virtual void DoActionFunc();

	void AddFunc(Func func);
	void AddDelayFunc(float delay, Func func);

protected:
	std::vector<Func> actionFuncs_;
	std::vector<std::pair<float, Func> > delayFuncs_;
	bool bSatisfied_{ false };
	bool bHasDelayFuncs_{false};
};

