#pragma once
#include "Action.h"

#include "Utility.h"

class FuncAction :
	public Action
{
public:
	FuncAction(Func func) :func_(func) {};
	void DoAction() override { func_(); }
protected:
	Func func_;
};

