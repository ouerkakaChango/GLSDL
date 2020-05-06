#include "Timeline.h"

#include "Action.h"

void Timeline::AddAction(float time, Action* action)
{
	if (action != nullptr)
	{
		action->SetTime(time);
		actions_.push_back(action);
	}
}

void Timeline::Start()
{
	now_ = 0.0f;
}

void Timeline::Update(float deltaTime)
{
	now_ += deltaTime;
	for (auto& action : actions_)
	{
		action->Check(now_);
	}
}