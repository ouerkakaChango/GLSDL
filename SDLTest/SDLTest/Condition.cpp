#include "Condition.h"

#include "God.h"

Condition::Condition()
{
}


Condition::~Condition()
{
}

void Condition::AddFunc(Func func)
{
	actionFuncs_.push_back(func);
}

void Condition::AddDelayFunc(float delay, Func func)
{
	delayFuncs_.push_back(std::make_pair(delay,func));
}

void Condition::DoActionFunc()
{
	//1.do actionFuncs immediately
	for (auto& func : actionFuncs_)
	{
		func();
	}

	//2.settimer do delayFuncs
	for (auto& i : delayFuncs_)
	{
		GOD.SetTimer(i.first,i.second);
	}
}