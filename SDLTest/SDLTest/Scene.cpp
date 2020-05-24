#include "Scene.h"

#include "Drawable.h"
#include "Timeline.h"
#include "ShowAction.h"
#include "EndAction.h"
#include "SoundAction.h"
#include "FuncAction.h"
#include "Effect.h"
#include "God.h"

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
	//äÖÈ¾¸üÐÂ
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
	auto showAction = new ShowAction(this, drawable);
	if (effect!=nullptr)
	{
		showAction->SetEffect(effect);
	}
	timeline_->AddAction(time, showAction);
}

void Scene::SetActive(bool active) 
{ 
	Activeable::SetActive(active);
	for (auto& drawable : drawables_)
	{
		drawable->SetActive(active);
	}

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