#include "Timeline.h"

#include "Action.h"
#include "FuncAction.h"
#include "Effect.h"

void Timeline::AddAction(float time, Action* action)
{
	if (action != nullptr)
	{
		action->SetTime(time);
		actions_.push_back(action);
	}
}

void Timeline::AddAction(float time, Func function)
{
	FuncAction* action = new FuncAction(function);
	AddAction(time, action);
}

void Timeline::AddEffect(float time, Effect* effect)
{
	effect->startInTimeline_ = now_ + time;
	effects_.push_back(effect);
}

void Timeline::Reset()
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

	for (auto& effect : effects_)
	{
		if (!effect->bTriggered_ && effect->startInTimeline_ >= now_)
		{
			effect->Start();
		}
		if (effect->GetActive())
		{
			effect->Update(deltaTime);
		}
	}
}