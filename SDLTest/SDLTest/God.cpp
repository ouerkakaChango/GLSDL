#include "God.h"

#include "Event.h"
#include "Object.h"
#include "SceneManager.h"
#include "Drawable.h"
#include "Material.h"

God& God::GetInstance()
{
	static God god;
	return god;
}

God::God()
{

}

void God::Init()
{
	defaultMaterial_ = new Material;
	defaultMaterial_->CompileShader("D:/HumanTree/code/quad.vs", "D:/HumanTree/code/quad.fs");
}

void God::BroadCast(Event* event)
{
	std::vector<Object*>* results = nullptr;
	if (eventMapVector_.Find(event->name_, results))
	{
		for (auto& object : *results)
		{
			object->HandleEvent(event);
		}
	}
}

void God::BindEvent(std::string eventName, Object* object)
{
	eventMapVector_.map_[eventName].push_back(object);
}

void God::AddPostDrawable(Drawable* drawable)
{
	if (drawable != nullptr)
	{
		drawable->SetActive(true);
		postDrawables_.push_back(drawable);
	}
}

void God::ChangePostDrawable(Drawable* oriDrawable, Drawable* newDrawable)
{
	oriDrawable->SetActive(false);
	newDrawable->SetActive(true);
	postDrawables_.remove(oriDrawable);
	postDrawables_.push_back(newDrawable);
}

void God::Update(float deltaTime)
{
	sceneManager_.Update(deltaTime);
	for (auto& drawable : postDrawables_)
	{
		drawable->Render();
	}
}

Material* God::CloneDefaultMaterial()
{
	return defaultMaterial_->Clone();
}