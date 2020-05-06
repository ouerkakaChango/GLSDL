#pragma once
#include "Action.h"

class SoundEffect;

class SoundAction : public Action
{
public:
	SoundAction(SoundEffect* sound) :sound_(sound) {};
	~SoundAction();

	void DoAction() override;
protected:
	SoundEffect* sound_{nullptr};
};

