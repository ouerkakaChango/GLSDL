#include "EffectChangeSize.h"

#include "Object.h"

EffectChangeSize::EffectChangeSize()
{
	curve_.SetInterp(Interp_Linear);
}


EffectChangeSize::~EffectChangeSize()
{
}

void EffectChangeSize::Start()
{
	Effect::Start();
	nowTime_ = 0.0f;
}

void EffectChangeSize::Update(float deltaTime)
{
	nowTime_ += deltaTime;
	obj_->ChangeSize(curve_.Get(nowTime_));
}