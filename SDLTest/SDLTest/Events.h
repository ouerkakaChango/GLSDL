#pragma once
#include "Event.h"

class Events
{
public:
	Events();
	~Events();
};

class LMB_Down :public Event
{
public:
	LMB_Down(int x, int y);
	~LMB_Down();

	int x_;
	int y_;
};

class Mouse_Move : public Event
{
public:
	Mouse_Move(int x, int y);
	~Mouse_Move();
	int x_;
	int y_;
};

class LMB_Up :public Event
{
public:
	LMB_Up(int x, int y);

	int x_;
	int y_;
};

class PPT_PageFlip : public Event
{
public:
	PPT_PageFlip(const std::string& groupName,unsigned inx, bool bLastPage);

	std::string groupName_;
	unsigned inx_;
	bool bLastPage_;
};