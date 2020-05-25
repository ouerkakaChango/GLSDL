#pragma once
#include "Nameable.h"
#include "Tree.h"

#include "Utility.h"

class Pass : public Nameable,public Tree<Pass>
{
public:
	void SetShader(const Path& vsPath, const Path& psPath);
};

