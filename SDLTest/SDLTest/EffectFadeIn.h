#pragma once
#include "Effect.h"

class Image;

class EffectFadeIn : public Effect
{
public:
	EffectFadeIn(Image* image, float fadeInTime);
	void Start() override;
	void Update(float deltaTime) override;
protected:
	Image* image_;
	float fadeInTime_;
	float nowTime_;
	float nowAlpha_;
};

