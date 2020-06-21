#pragma once

#include "Int.h"
#include "Utility.h"

class Scene;
class SceneTransition;
class Effect;
class RenderTexture;

class SceneManager
{
public:
	Scene* InsertScene(Path bgPath = "");	//(deprecated)设置背景图片新建scene插入
	Scene* AddScene(Path bgPath = "");
	void AddTransition(const Int<2>& int2, SceneTransition* transition);
	void Update(float deltaTime);

	void ToNext();
	void SetSceneActive(int inx,bool active);
	int GetNowSceneIndex() { return sceneInx_; }
	const Scene* GetNowScene() { return scenes_[sceneInx_]; }
	void AddCrossEffect(Effect* effect);			//跨scene的effect
	void ClearScenes();
	RenderTexture* GetSceneRT(unsigned inx);

	//debug
	void JumpToScene(int sceneInx);
protected:
	std::vector<Scene*> scenes_;
	std::vector<Effect*> crossEffects_;
	std::map<Int<2>,SceneTransition*> transitions_;
	int sceneInx_{ -1 };

	friend class SceneTransition;
};

