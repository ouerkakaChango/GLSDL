#include "SceneManager.h"

#include "Scene.h"
#include "SceneTransition.h"
#include "God.h"
#include "Image.h"
#include "Effect.h"
#include "ShaderImage.h"
#include "SceneShaderImage.h"

Scene* SceneManager::InsertScene(Path bgPath)
{
	Scene* newScene = new Scene;
	if (!bgPath.empty())
	{
		unsigned bgW, bgH;
		bgW = GOD.gameConfig_.Get<int>("windowWidth");
		bgH = GOD.gameConfig_.Get<int>("windowHeight");
		Image *bgImg =  new Image(bgW, bgH);
		if (bgImg->Load(bgPath))
		{
			bgImg->SetPosition(bgW / 2, bgH / 2 );
			newScene->AddDrawable(bgImg);
		}
	}
	scenes_.push_back(newScene);
	return newScene;
}

Scene* SceneManager::AddScene(Path bgPath)
{
	Scene* newScene = new Scene;
	if (!bgPath.empty())
	{
		unsigned bgW, bgH;
		bgW = GOD.gameConfig_.Get<int>("windowWidth");
		bgH = GOD.gameConfig_.Get<int>("windowHeight");
		Image *img = new Image(bgW, bgH);
		if (img->ReadFile(bgPath))
		{
			img->SetPosition(bgW / 2, bgH / 2);
			SceneShaderImage* sceneImg = new SceneShaderImage(img);
			sceneImg->name_ = bgPath;
			newScene->sceneColorShaderImg_ = sceneImg;
		}
	}
	scenes_.push_back(newScene);
	return newScene;
}

void SceneManager::AddTransition(const Int<2>& int2, SceneTransition* transition)
{
	transition->frontInx_ = int2[0];
	transition->nextInx_ = int2[1];
	transitions_[int2] = transition;
	//auto active first transition
	if (int2[0] == -1)
	{
		transition->SetActive(true);
	}
}

void SceneManager::Update(float deltaTime)
{
	for (auto& scene : scenes_)
	{
		if (scene->GetActive())
		{
			scene->Update(deltaTime);
		}
	}
	for (auto& it : transitions_)
	{
		auto transition = it.second;
		if (transition->GetActive())
		{
			int frontInx = it.first[0];
			int nextInx = it.first[1];
			if (frontInx == sceneInx_)
			{
				sceneInx_ = nextInx;
			}
			transition->Update(deltaTime);
		}
	}
	for (auto& effect : crossEffects_)
	{
		if (effect->GetActive())
		{
			effect->Update(deltaTime);
		}
	}
}

void SceneManager::ToNext()
{
	for (auto& it : transitions_)
	{
		auto transition = it.second;
		int frontInx = it.first[0];
		int nextInx = it.first[1];
		if (frontInx == sceneInx_)
		{
			sceneInx_ = nextInx;
			transition->SetActive(true);
			break;
		}
	}
}

void SceneManager::SetSceneActive(int inx, bool active)
{
	if (inx <= -1 || inx >= scenes_.size())
	{

	}
	else
	{
		scenes_[inx]->SetActive(active);
	}
}

void SceneManager::AddCrossEffect(Effect* effect)
{
	if (effect != nullptr)
	{
		effect->Start();
		crossEffects_.push_back(effect);
	}
}

void SceneManager::ClearScenes()
{
	scenes_.clear();
}

void SceneManager::JumpToScene(int sceneInx)
{
	for (unsigned i = 0; i < sceneInx; i++)
	{
		scenes_[i]->SetActive(false);
		transitions_[Int<2>(-1, 0)]->SetActive(false);

	}
	sceneInx_ = sceneInx;
	scenes_[sceneInx_]->SetActive(true);
}

RenderTexture* SceneManager::GetSceneRT(unsigned inx)
{
	return scenes_[inx]->sceneColorShaderImg_->GetRT();
}