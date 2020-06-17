#pragma once
#include "Object.h"

#include "GraphicDefs.h"

class Scene;
class RenderTexture;
class DrawCall;

class Drawable : public Object
{
public:
	Drawable();
	~Drawable();
	void SetOwnerScene(Scene* scene) { scene_ = scene; }
	virtual void Render() {};	//oldDraw
	virtual void GetDrawcall() {}
	virtual void SetSceneRT(RenderTexture* sceneRT);

	void SetDrawCallChannel(DrawCallChannel newChannel);
protected:
	void CommitDrawCall();

	DrawCall*		dc_{nullptr};
	Scene*			scene_{nullptr};
	RenderTexture*	sceneRT_{ nullptr };
	DrawCallChannel dcChannel_{ DrawCall_Passive };
};

