#pragma once
#include <vector>

#include "Utility.h"
#include "Activeable.h"

class Drawable;
class Timeline;
class Effect;
class SoundEffect;

class Scene : public Activeable
{
public:
	Scene();
	~Scene();
	void SetActive(bool active) override;

	void Render();
	void Update(float deltaTime);

	//向Scene添加drawable,可能是稍后变active的，也可能需要直接设置是否active
	void AddDrawable(Drawable*);
	void AddDrawable(Drawable* drawable,bool bActive);
	//演员上场，可决定出场方式和在Timeline中的位置
	void Show(Drawable* drawable,float time=0.0f, Effect* effect=nullptr);
	//设置时长自动落幕
	void SetAutoEnd(float time);
	void AddSound(SoundEffect* sound, float time);
	void AddCustomAction(float time, Func func);

	Drawable* GetBackgroundDrawable() { return backgroundDrawable_; }
protected:
	std::vector<Drawable*> drawables_;
	std::vector<Effect*> effects_;
	Timeline* timeline_;
	Drawable* backgroundDrawable_{ nullptr };

	void AddEffect(Effect* effect);
	friend class ShowAction;
	friend class SceneManager;
	friend class SceneTransition;
};

