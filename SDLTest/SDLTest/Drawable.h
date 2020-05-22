#pragma once
#include "Object.h"

class Scene;
class Drawable : public Object
{
public:
	Drawable();
	~Drawable();
	void SetOwnerScene(Scene* scene) { scene_ = scene; }
	virtual void Render() = 0;
protected:
	Scene* scene_{nullptr};
};

