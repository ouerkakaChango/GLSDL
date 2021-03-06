#pragma once
#include <vector>

#include "Utility.h"
#include "Activeable.h"

class Drawable;
class Timeline;
class Effect;
class SoundEffect;
class SceneShaderImage;
class Condition;

class Scene : public Activeable
{
public:
	Scene();
	~Scene();
	void SetActive(bool active) override;

	void Render();
	void Update(float deltaTime);

	void AddDrawable(Drawable*); //(deprecated) for oldDraw
	void AddDrawable(Drawable* drawable,bool bActive);
	//演员上场，可决定出场方式和在Timeline中的位置
	void Show(Drawable* drawable,float time=0.0f, Effect* effect=nullptr);
	void ConditionShow(Drawable* drawable, Condition* condition);
	//设置时长自动落幕
	void SetAutoEnd(float time);
	void AddSound(SoundEffect* sound, float time);
	void AddCustomAction(float time, Func func);

	SceneShaderImage* GetSceneImg() { return sceneColorShaderImg_; }
protected:
	void SaveDrawableState();
	void RestoreDrawableState();

	std::vector<Drawable*> drawables_;
	std::vector<Effect*> effects_;
	std::map<Drawable*, bool> drawableStates_;
	Timeline* timeline_;
	SceneShaderImage* sceneColorShaderImg_{ nullptr };
	bool bFirstActive_{true};

	void AddEffect(Effect* effect);
	friend class ShowAction;
	friend class SceneManager;
	friend class SceneTransition;
};

