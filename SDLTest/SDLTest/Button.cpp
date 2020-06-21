#include "Button.h"

#include "Image.h"
#include "God.h"
#include "Event.h"
#include "LMB_Down.h"

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


Button::~Button()
{
}

void Button::Render()
{
	if (bRender_)
	{
		image_->Render();
	}
}