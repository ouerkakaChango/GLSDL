#pragma once
#include "Activeable.h"
#include "Nameable.h"

#include "Utility.h"
#include "Math2D.h"

#include <functional>

class Event;

typedef std::function<void(Event*)> EventHandler;
typedef std::function<bool(Event*)> EventGate;

class Object : public Activeable,public Nameable
{
public:
	Object();
	void HandleEvent(Event* event); //通常只由GOD调用
	void BindEventHandler(std::string eventName);
	void BindEventHandler(std::string eventName,EventHandler handler);
	void BindEventGate(std::string eventName, EventGate gate);

	virtual void ChangeSize(Vec2 size) {};
protected:
	SDL_Renderer* renderer_;
	StringMap<EventHandler> eventHandlerMap_;
	StringMap<EventGate> eventGateMap_;

};

#define XClass(className) \
public: \
static std::string TypeName() {return #className;}