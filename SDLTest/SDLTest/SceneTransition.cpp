#include "SceneTransition.h"

#include "Image.h"
#include "God.h"
#include "ShaderImage.h"
#include "FuncAction.h"

#include "Debug.h"

SceneTransition::SceneTransition(const string& effectName,float transitionTime):effectName_(effectName),transitionTime_(transitionTime)
{
	//??? 转场特效
	if (effectName_ == "fadeIn"||effectName =="fadeOutIn")
	{
		blackImg_ = new Image(GOD.gameConfig_.Get<int>("windowWidth"), GOD.gameConfig_.Get<int>("windowHeight"));
	}
}

SceneTransition::SceneTransition(const string& effectName, Params<float> params) :effectName_(effectName)
{
	if (effectName_ == "fastBlackWithBlurIn")
	{
		Func func1 = [&]()
		{
			GOD.sceneManager_.SetSceneActive(frontInx_, false);
			GOD.blackBackground_->SetActive(true);
		};
		Func func2 = [&]()
		{
			GOD.blackBackground_->SetActive(false);
			GOD.sceneManager_.SetSceneActive(nextInx_, true);
		};
		FuncAction* action1 = new FuncAction(func1);
		FuncAction* action2 = new FuncAction(func2);
		timeline_.AddAction(0, action1);
		timeline_.AddAction(params[0],action2);
	}
}

void SceneTransition::Update(float deltaTime)
{
	//??? 转场特效
	if (effectName_ == "fadeIn")
	{
		if (blackImg_->texture_ == nullptr && blackImg_->Load("D:/HumanTree/black.png"))
		{
			blackImg_->SetPosition(blackImg_->GetWidth() / 2, blackImg_->GetHeight() / 2);
			GOD.sceneManager_.SetSceneActive(nextInx_,true);
		}
		static float past = 0.0f;
		past += deltaTime;
		auto* tex = blackImg_->texture_;
		float nowa = 1 - past / 3.0f;
		if (nowa <= 0)
		{
			nowa = 0;
			bActive_ = false;
		}
		SDL_SetTextureAlphaMod(tex, 255 * nowa);
		blackImg_->Render();
	}
	else if (effectName_ == "fadeOutIn")
	{//??? 转场特效
		if (blackImg_->texture_ == nullptr && blackImg_->Load("D:/HumanTree/black.png"))
		{
			blackImg_->SetPosition(blackImg_->GetWidth() / 2, blackImg_->GetHeight() / 2);
			GOD.sceneManager_.SetSceneActive(frontInx_,true);
		}
		static bool first = true;
		auto myFunc =[&](float k)
		{
			if (k > 0 && k <= 0.5f)
			{
				return k/0.5f;
			}
			else
			{
				if (first)
				{
					GOD.sceneManager_.SetSceneActive(frontInx_, false);
					GOD.sceneManager_.SetSceneActive(nextInx_, true);
					first = false;
				}
				return (1 - k)/0.5f;
			}
		};

		static float past = 0.0f;
		past += deltaTime;
		auto* tex = blackImg_->texture_;
		float t = 0;
		float k = past / transitionTime_;
		float nowa = myFunc(k);
		if (k >=0.99f)
		{
			nowa = 0;
			past = 0.0f;
			first = true;
			bActive_ = false;
		}
		Uint8 a = static_cast<int>(255 * nowa);
		SDL_SetTextureAlphaMod(tex, a);
		blackImg_->Render();
	}
	else if (effectName_ == "fastBlackWithBlurIn")
	{
		timeline_.Update(deltaTime);
	}
}

