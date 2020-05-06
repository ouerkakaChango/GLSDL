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

	//直接向Scene添加drawable
	void AddDrawable(Drawable*);
	//演员上场，可决定出场方式和在Timeline中的位置
	void Show(Drawable* drawable,float time=0.0f, Effect* effect=nullptr);
	//设置时长自动落幕
	void SetAutoEnd(float time);
	void AddSound(SoundEffect* sound, float time);
	void AddCustomAction(float time, Func func);
protected:
	std::vector<Drawable*> drawables_;
	std::vector<Effect*> effects_;
	Timeline* timeline_;

	void AddEffect(Effect* effect);
	friend class ShowAction;
};

