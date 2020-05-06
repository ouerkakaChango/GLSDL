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
			{//������Gate�������ȼ����������Gate�������ٵ���
				auto gateFunc = *gate;
				if (gateFunc(event))
				{
					(*func)(event);
				}
			}
			else
			{//δ�󶨹�Gate������ֱ�ӵ���
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