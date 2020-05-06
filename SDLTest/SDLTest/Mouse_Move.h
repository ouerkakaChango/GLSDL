#pragma once
#include "Event.h"
class Mouse_Move :
	public Event
{
public:
	Mouse_Move(int x, int y);
	~Mouse_Move();
	int x_;
	int y_;
};

