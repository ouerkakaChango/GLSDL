#include "God.h"

#include "Event.h"
#include "Object.h"
#include "SceneManager.h"
#include "Drawable.h"
#include "Material.h"
#include "Image.h"
#include "ShaderImage.h"
#include "Timeline.h"

God& God::GetInstance()
{
	static God god;
	return god;
}

God::God()
{
	timeline_ = new Timeline;
	timeline_->Reset();
}

void God::Init()
{
	defaultMaterial_ = new Material;
	defaultMaterial_->CompileShader("D:/HumanTree/code/quad.vs", "D:/HumanTree/code/quad.fs");

	unsigned bgW, bgH;
	bgW = GOD.gameConfig_.Get<int>("windowWidth");
	bgH = GOD.gameConfig_.Get<int>("windowHeight");
	Image* blackBGImg = new Image(bgW,bgH);
	blackBGImg->ReadFile("D:/HumanTree/ResEngine/black.png");
	blackBGImg->SetPosition(bgW / 2, bgH / 2);
	blackBackground_ = new ShaderImage(blackBGImg);
	blackBackground_->name_ = "blackBackground_";
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
	timeline_->Update(deltaTime);
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

void God::SetTimer(float delay, Func function)
{

	timeline_->AddAction(timeline_->Now()+delay, function);
}

void God::GetDrawcalls()
{
	passiveDrawcalls_.clear();
	postDrawcalls_.clear();
	
	for (auto& drawable : drawcallDrawables_)
	{
		drawable->GetDrawcall();
	}
}