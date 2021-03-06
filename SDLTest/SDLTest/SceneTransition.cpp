#include "SceneTransition.h"

#include "Scene.h"
#include "Image.h"
#include "God.h"
#include "ShaderImage.h"
#include "FuncAction.h"
#include "Material.h"
#include "Pass.h"
#include "UsualEffects.h"
#include "SceneShaderImage.h"
#include "DrawCall.h"

#include "Debug.h"

SceneTransition::SceneTransition(const string& effectName,float transitionTime):effectName_(effectName),transitionTime_(transitionTime)
{
	//??? 转场特效
	if (effectName_ == "fadeIn"||effectName =="fadeOutIn")
	{
		blackImg_ = new Image(GOD.gameConfig_.Get<int>("windowWidth"), GOD.gameConfig_.Get<int>("windowHeight"));
	}
	else if(effectName_ == "glFadeOutIn")
	{

		Func func1 = [&]()
		{

			auto& black = GOD.fadeBlackCurtain_;
			black->dc_->name_ = "fadeBlackDC";
			black->SetActive(true);
			//black->SetSceneRT(GOD.sceneManager_.GetSceneRT(frontInx_));

			EffectShaderParam* paramEffect = new EffectShaderParam;
			paramEffect->Bind(black->material_, "alpha");

			paramEffect->AddPoint(0.0f, 0.0f);
			paramEffect->AddPoint(transitionTime_ / 2.0f, 1.0f);
			paramEffect->AddPoint(transitionTime_, 0.0f);
			//???
			paramEffect->name_ = "fadeEff";
			timeline_.AddEffect(timeline_.Now(), paramEffect);
		};
		//mid time close front scene,activate next scene
		Func func2 = [&]()
		{
			GOD.sceneManager_.SetSceneActive(frontInx_, false);
			GOD.sceneManager_.SetSceneActive(nextInx_, true);
		};
		//end time close curtain
		Func func3 = [&]()
		{
			GOD.fadeBlackCurtain_->SetActive(false);
			SetActive(false);
		};
		timeline_.AddAction(0, func1);
		timeline_.AddAction(transitionTime_ /2.0f, func2);
		timeline_.AddAction(transitionTime_, func3);
	}
	else
	{
		abort();
	}
}

SceneTransition::SceneTransition(const string& effectName, Params<float> params) :effectName_(effectName)
{
	if (effectName_ == "fastBlackWithBlurIn")
	{
		static Pass* GBlurOncePass = new Pass;
		GBlurOncePass->SetShader("D:/HumanTree/code/quadRT.vs", "D:/HumanTree/code/gaussianBlur.fs");
		static Pass* blur = new Pass;
		blur->AddChild(GBlurOncePass, 30);
		static Material* quadWithBlurMat = new Material;
		sure(quadWithBlurMat->CompileShader("D:/HumanTree/code/quad.vs", "D:/HumanTree/code/quadWithBlur.fs"));

		//end last scene immediately,drop black
		Func func1 = [&]()
		{
			GOD.sceneManager_.SetSceneActive(frontInx_, false);
			GOD.fastBlackCurtain_->SetActive(true);
		};

		//black end,show next scene,but with blur duration effect
		Func func2 = [&,params]()
		{
			GOD.fastBlackCurtain_->SetActive(false);
			GOD.sceneManager_.SetSceneActive(nextInx_, true);

			{
				auto* newScene = GOD.sceneManager_.scenes_[nextInx_];
				SceneShaderImage* bg = newScene->GetSceneImg();
				bg->ChangeMaterial(quadWithBlurMat);
				bg->UsePass(blur);

				EffectShaderParam* paramEffect = new EffectShaderParam;
				paramEffect->Bind(quadWithBlurMat, "blur");
				paramEffect->AddPoint(0.0f, 1.0f);
				paramEffect->AddPoint(params[1], 0.0f);
				timeline_.AddEffect(timeline_.Now(), paramEffect);
			}
		};
		Func func3 = [&]()
		{
			auto* newScene = GOD.sceneManager_.scenes_[nextInx_];
			SceneShaderImage* bg = newScene->GetSceneImg();
			bg->ChangeMaterial(GOD.CloneDefaultMaterial());
			bg->UsePass(nullptr);
			SetActive(false);
		};
		FuncAction* action1 = new FuncAction(func1);	//转黑幕
		FuncAction* action2 = new FuncAction(func2);	//开高斯模糊渐变
		FuncAction* action3 = new FuncAction(func3);	//切为正常材质，减少dc
		timeline_.AddAction(0, action1);			
		timeline_.AddAction(params[0],action2);
		timeline_.AddAction(params[0] + params[1], action3);
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
	else if (effectName_ == "glFadeOutIn")
	{
		timeline_.Update(deltaTime);
	}
}

