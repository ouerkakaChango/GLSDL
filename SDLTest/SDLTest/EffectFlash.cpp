#include "EffectFlash.h"

#include "Image.h"

#include "Debug.h"

EffectFlash::EffectFlash(Image* image, float frequency, float duration) :Effect(), image_(image), duration_(duration)
{
	interval_ = 1 / frequency;
	bLoop_ = duration < 0;
}


EffectFlash::~EffectFlash()
{
}

void EffectFlash::Start()
{
	Effect::Start();
	nowTime_ = 0.0f;
	nowAlpha_ = 0.0f;
}

void EffectFlash::Update(float deltaTime)
{
	float k = nowTimeInInterval_ / interval_;;
	nowAlpha_ = FlashFunc(k);
	auto tex = image_->texture_;
	SDL_SetTextureAlphaMod(tex, 255 * nowAlpha_);
	image_->Render();

	nowTimeInInterval_ += deltaTime;
	if (nowTimeInInterval_ > interval_)
	{
		nowTimeInInterval_ = 0.0f;
	}

	nowTime_ += deltaTime;
	if (!bLoop_ && nowTime_ > duration_)
	{
		bActive_ = false;
	}
}