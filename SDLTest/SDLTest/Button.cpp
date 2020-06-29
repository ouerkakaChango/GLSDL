#include "Button.h"

#include "Image.h"
#include "God.h"
#include "Events.h"
#include "ShaderImage.h"
#include "Material.h"

Button::Button(bool bRender)
	:bRender_(bRender)
{
	GOD.BindEvent("LMB_Down", this);
	EventGate gate1 = [&](Event* event)
	{
		LMB_Down* e = (LMB_Down*)event;
		if (InRect(e->x_,e->y_, image_->GetSDLRect()))
		{
			return true;
		}
		else
		{
			return false;
		}
	};
	BindEventGate("LMB_Down", gate1);
}

Button::Button(Image* img)
	:image_(img),
	bRender_(true)
{
	GOD.BindEvent("LMB_Down", this);
	EventGate gate1 = [&](Event* event)
	{
		LMB_Down* e = (LMB_Down*)event;
		if (InRect(e->x_, e->y_, image_->GetSDLRect()))
		{
			return true;
		}
		else
		{
			return false;
		}
	};
	BindEventGate("LMB_Down", gate1);
	sImg_ = new ShaderImage(image_);
	sImg_->material_->SetBlendType(Blend_Alpha);
	sImg_->name_ = "RButton";
}


Button::~Button()
{
}

void Button::Render()
{
	if (bActive_&&bRender_)
	{
		if (GOD.bOldDraw_)
		{
			image_->Render();
		}
	}
}

void Button::SetActive(bool active)
{
	Drawable::SetActive(active);
	if (sImg_ != nullptr)
	{
		sImg_->SetActive(active);
	}
}

void Button::SetSceneRT(RenderTexture* sceneRT)
{
	if (sImg_ != nullptr)
	{
		sImg_->SetSceneRT(sceneRT);
	}
}