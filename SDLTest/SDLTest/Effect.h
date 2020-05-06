#pragma once
#include "Activeable.h"

class Effect : public Activeable
{
public:
	Effect();
	~Effect();
	virtual void Start()=0;
	virtual void Update(float deltaTime) = 0;
};

