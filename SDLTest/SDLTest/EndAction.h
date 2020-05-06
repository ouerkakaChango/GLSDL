#pragma once
#include "Action.h"
class EndAction :
	public Action
{
public:
	EndAction();
	~EndAction();
	void DoAction() override;
};

