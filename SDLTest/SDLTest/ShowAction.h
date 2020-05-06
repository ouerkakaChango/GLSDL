#pragma once
#include "Action.h"

class Drawable;
class Scene;
class Effect;

class ShowAction :public Action
{
public:
	ShowAction(Scene* scene, Drawable* drawable);
	void DoAction() override;
	void SetEffect(Effect* effect);
protected:
	Scene* scene_;
	Drawable* drawable_;
	Effect* effect_{nullptr};
};
