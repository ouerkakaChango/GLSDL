#pragma once
#include "Effect.h"

class Image;

class EffectFlash : public Effect
{
public:
	//��durationС��0������Ϊ����
	EffectFlash(Image* image, float frequency, float duration=-1.0f);
	~EffectFlash();
	void Start() override;
	void Update(float deltaTime) override;
protected:
	Image* image_;
	float interval_;
	float duration_;
	bool bLoop_;
	float nowTime_;
	float nowTimeInInterval_;
	float nowAlpha_;
};

