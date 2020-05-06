#include "Object.h"

#include "God.h"
#include "Event.h"

Object::Object()
{
	renderer_ = GOD.GetRenderer();
}

void Object::HandleEvent(Event* event)
{
	if (bActive_)
	{
		EventHandler* func = nullptr;
		if (eventHandlerMap_.Find(event->name_, func))
		{
			EventGate* gate = nullptr;
			if (eventGateMap_.Find(event->name_, gate))
			{//若绑定了Gate函数，先检查满不满足Gate条件，再调用
				auto gateFunc = *gate;
				if (gateFunc(event))
				{
					(*func)(event);
				}
			}
			else
			{//未绑定过Gate函数，直接调用
				(*func)(event);
			}
		}
	}
	else
	{
		auto aaa = 1;
	}
}

void Object::BindEventHandler(std::string eventName, EventHandler handler)
{
	eventHandlerMap_.map_[eventName] = handler;
}

void Object::BindEventHandler(std::string eventName) {
	EventHandler emptyFunc = [&](Event* event)
	{
	};
	BindEventHandler(eventName, emptyFunc);
}

void Object::BindEventGate(std::string eventName, EventGate gate)
{
	eventGateMap_.map_[eventName] = gate;
}