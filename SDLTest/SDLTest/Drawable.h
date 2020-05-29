#pragma once
#include "Object.h"

class Scene;
class RenderTexture;
class Drawable : public Object
{
public:
	Drawable();
	~Drawable();
	void SetOwnerScene(Scene* scene) { scene_ = scene; }
	virtual void Render() = 0;
	virtual void GetDrawcall() {}
	virtual void SetSceneRT(RenderTexture* sceneRT);
protected:
	Scene* scene_{nullptr};
	RenderTexture* sceneRT_{ nullptr };
};

