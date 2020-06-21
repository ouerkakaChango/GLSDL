#pragma once
#include "Activeable.h"
#include "Nameable.h"

class Effect : public Activeable,public Nameable
{
public:
	Effect();
	~Effect();
	virtual void Start();
	virtual void Update(float deltaTime) = 0;
protected:
	float startInTimeline_{ 0.0f };
	bool bTriggered_{ false };

	friend class Timeline;
};

