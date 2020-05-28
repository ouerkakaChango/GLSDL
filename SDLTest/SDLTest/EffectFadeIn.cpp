#include "EffectFadeIn.h"

#include "Image.h"
#include "Utility.h"

#include "Debug.h"

EffectFadeIn::EffectFadeIn(Image* image, float fadeInTime):Effect(),image_(image), fadeInTime_(fadeInTime)
{
	
}

void EffectFadeIn::Start()
{
	Effect::Start();
	nowTime_ = 0.0f;
	nowAlpha_ = 0.0f;
}

void EffectFadeIn::Update(float deltaTime)
{
	auto tex = image_->texture_;
	SDL_SetTextureAlphaMod(tex, 255 * nowAlpha_);
	nowTime_ += deltaTime;
	if (nowTime_ >= fadeInTime_)
	{
		bActive_ = false;
		return;
	}
	nowAlpha_ =  nowTime_ / fadeInTime_;
}