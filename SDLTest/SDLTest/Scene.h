#pragma once
#include <vector>

#include "Utility.h"
#include "Activeable.h"

class Drawable;
class Timeline;
class Effect;
class SoundEffect;
class SceneShaderImage;

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
	//��Ա�ϳ����ɾ���������ʽ����Timeline�е�λ��
	void Show(Drawable* drawable,float time=0.0f, Effect* effect=nullptr);
	//����ʱ���Զ���Ļ
	void SetAutoEnd(float time);
	void AddSound(SoundEffect* sound, float time);
	void AddCustomAction(float time, Func func);

	SceneShaderImage* GetSceneImg() { return sceneColorShaderImg_; }
protected:
	std::vector<Drawable*> drawables_;
	std::vector<Effect*> effects_;
	Timeline* timeline_;
	SceneShaderImage* sceneColorShaderImg_{ nullptr };

	void AddEffect(Effect* effect);
	friend class ShowAction;
	friend class SceneManager;
	friend class SceneTransition;
};

