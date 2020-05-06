#include "Touchable.h"

#include "God.h"
#include "Mouse_Move.h"
#include "Image.h"

Touchable::Touchable()
{
	GOD.BindEvent("Mouse_Move", this);
	EventGate gate1 = [&](Event* event)
	{
		Mouse_Move* e = (Mouse_Move*)event;
		if (InRect(e->x_, e->y_, image_->GetSDLRect()))
		{
			if (!lastTouch_)
			{
				beginOverlapFunc_();
			}
			lastTouch_ = true;
			return true;
		}
		else
		{
			if (lastTouch_)
			{
				endOverlapFunc_();
			}
			lastTouch_ = false;
			return false;
		}
	};
	BindEventGate("Mouse_Move", gate1);
}

void Touchable::Render()
{
	image_->Render();
}