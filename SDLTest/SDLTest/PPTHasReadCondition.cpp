#include "PPTHasReadCondition.h"

#include "God.h"
#include "Events.h"

PPTHasReadCondition::PPTHasReadCondition()
{
}


PPTHasReadCondition::~PPTHasReadCondition()
{
}

void PPTHasReadCondition::Bind(ShaderPPT* ppt, const std::string& groupName)
{
	sure(ppt != nullptr);
	ppt_ = ppt;
	GOD.BindEvent("PPT_PageFlip", this);
	EventGate gate1 = [&](Event* event)
	{
		PPT_PageFlip* e = (PPT_PageFlip*)event;
		return e->bLastPage_;
	};
	BindEventGate("PPT_PageFlip", gate1);

	EventHandler func = [&](Event* event)
	{
		DoActionFunc();
	};
	BindEventHandler("PPT_PageFlip", func);
}