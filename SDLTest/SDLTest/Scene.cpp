#include "Scene.h"

#include "Drawable.h"
#include "Timeline.h"
#include "ShowAction.h"
#include "EndAction.h"
#include "SoundAction.h"
#include "FuncAction.h"
#include "Effect.h"
#include "God.h"
#include "SceneShaderImage.h"
#include "Condition.h"

Scene::Scene()
{
	timeline_ = new Timeline;
}


Scene::~Scene()
{

}

void Scene::AddDrawable(Drawable* drawable)
{
	if (drawable != nullptr)
	{
		drawable->SetOwnerScene(this);;
		drawables_.push_back(drawable);
	}
}

void Scene::AddDrawable(Drawable* drawable, bool bActive)
{
	if (drawable != nullptr)
	{
		drawable->SetOwnerScene(this);
		drawable->SetActive(bActive);
		drawables_.push_back(drawable);
	}
}

void Scene::AddEffect(Effect* effect)
{
	if (effect != nullptr)
	{
		effects_.push_back(effect);
	}
}

void Scene::Update(float deltaTime)
{
	timeline_->Update(deltaTime);
	for (auto& effect : effects_)
	{
		if (effect->GetActive())
		{
			effect->Update(deltaTime);
		}
	}
	//渲染更新 (deprecated)
	Render();
}

void Scene::Render()
{
	for (auto& drawable : drawables_)
	{
		drawable->Render();
	}
}

void Scene::Show(Drawable* drawable, float time , Effect* effect )
{
	if (sceneColorShaderImg_ != nullptr)
	{
		drawable->SetSceneRT(sceneColorShaderImg_->GetRT());
	}
	auto showAction = new ShowAction(this, drawable);
	showAction->name_ = drawable->name_+" ShowAction";
	if (effect!=nullptr)
	{
		showAction->SetEffect(effect);
	}
	timeline_->AddAction(time, showAction);
}

void Scene::SetActive(bool active) 
{ 
	Activeable::SetActive(active);
	if (sceneColorShaderImg_ != nullptr)
	{
		sceneColorShaderImg_->SetActive(active);
	}

	if (!active)
	{//save state when close scene
		SaveDrawableState();
	}

	if (active)
	{
		if (bFirstActive_)
		{
			for (auto& drawable : drawables_)
			{
				drawable->SetActive(active);
			}
			bFirstActive_ = false;
		}
		else
		{//restore state when reopen scene
			RestoreDrawableState();
		}
	}
	else
	{
		for (auto& drawable : drawables_)
		{
			drawable->SetActive(false);
		}
	}

	//??? 忘了为啥要这样了
	if (active)
	{
		timeline_->Reset();
	}
}

void Scene::SetAutoEnd(float time)
{
	auto endAction = new EndAction;
	timeline_->AddAction(time, endAction);
}

void Scene::AddSound(SoundEffect* sound, float time)
{
	auto soundAction = new SoundAction(sound);
	timeline_->AddAction(time, soundAction);
}

void Scene::AddCustomAction(float time, Func func)
{
	auto funcAction = new FuncAction(func);
	timeline_->AddAction(time, funcAction);
}

void Scene::ConditionShow(Drawable* drawable, Condition* condition)
{
	condition->AddFunc(
		[&,drawable]() mutable
	{
		Show(drawable, 0);
	});
}

void Scene::SaveDrawableState()
{
	for (auto& i : drawables_)
	{
		drawableStates_[i] = i->GetActive();
	}
}

void Scene::RestoreDrawableState()
{
	for (auto& i : drawables_)
	{
		i->SetActive(drawableStates_[i]);
	}
}