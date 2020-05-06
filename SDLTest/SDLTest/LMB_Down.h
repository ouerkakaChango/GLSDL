#pragma once
#include "Event.h"

class LMB_Down :public Event
{
public:
	LMB_Down(int x, int y);
	~LMB_Down();

	int x_;
	int y_;
};

