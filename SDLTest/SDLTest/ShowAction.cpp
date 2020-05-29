#include "ShowAction.h"

#include "Scene.h"
#include "Drawable.h"
//这里要添加可能实行的effect子类
#include "EffectFadeIn.h"

#include "Debug.h"

ShowAction::ShowAction(Scene* scene, Drawable* drawable):scene_(scene),drawable_(drawable)
{

}

void ShowAction::SetEffect(Effect* effect)
{
	effect_ = effect;
	scene_->AddEffect(effect);
}

void ShowAction::DoAction()
{
	if (name_ == "testSImg ShowAction")
	{
		auto v = 1;
	}
	if (effect_!=nullptr)
	{
		effect_->Start();
	}
	scene_->AddDrawable(drawable_,true);
	std::cout << "do show action\n";
}