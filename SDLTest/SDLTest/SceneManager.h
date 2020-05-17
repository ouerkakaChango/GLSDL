#pragma once

#include "Int.h"
#include "Utility.h"

class Scene;
class SceneTransition;
class Effect;

class SceneManager
{
public:
	//���ã����ñ���ͼƬ�½�scene����
	Scene* InsertScene(Path bgPath = "");
	void AddTransition(const Int<2>& int2, SceneTransition* transition);
	void Update(float deltaTime);

	void ToNext();
	void SetSceneActive(int inx,bool active);
	int GetNowSceneIndex() { return sceneInx_; }
	void AddCrossEffect(Effect* effect);			//��scene��effect
	void ClearScenes();

protected:
	std::vector<Scene*> scenes_;
	std::vector<Effect*> crossEffects_;
	std::map<Int<2>,SceneTransition*> transitions_;
	int sceneInx_{ -1 };
};

