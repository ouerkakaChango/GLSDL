#include "Events.h"



Events::Events()
{
}


Events::~Events()
{
}

LMB_Down::LMB_Down(int x, int y) :x_(x), y_(y)
{
	name_ = "LMB_Down";
}

LMB_Down::~LMB_Down()
{
}

Mouse_Move::Mouse_Move(int x, int y) :x_(x), y_(y)
{
	name_ = "Mouse_Move";
}


Mouse_Move::~Mouse_Move()
{
}

LMB_Up::LMB_Up(int x, int y) :x_(x), y_(y)
{
	name_ = "LMB_Up";
}

PPT_PageFlip::PPT_PageFlip(const std::string& groupName, unsigned inx, bool bLastPage)
	:groupName_(groupName),
	inx_(inx),
	bLastPage_(bLastPage)
{
	name_ = "PPT_PageFlip";
}