#pragma once
#include "Condition.h"

#include "Utility.h"

class ShaderPPT;

class PPTHasReadCondition : public Condition
{
public:
	PPTHasReadCondition();
	~PPTHasReadCondition();
	void Bind(ShaderPPT* ppt, const std::string& groupName);
protected:
	ShaderPPT* ppt_{nullptr};
	std::string groupName_;
};

