#pragma once
#include "Object.h"

class Drawable : public Object
{
public:
	Drawable();
	~Drawable();
	virtual void Render() = 0;
};

