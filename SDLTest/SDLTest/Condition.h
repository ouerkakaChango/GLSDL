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

	std::vector<Func> actionFuncs_;
};

