#include "Condition.h"



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

void Condition::DoActionFunc()
{
	for (auto& func : actionFuncs_)
	{
		func();
	}
}