#include "MorphGrid.h"



MorphGrid::MorphGrid(Vec2 v1, Vec2 v2, Vec2 v3, Vec2 v4,int xCell, int yCell)
	:v1_(v1),
	v2_(v2),
	v3_(v3),
	v4_(v4),
	xCell_(xCell),
	yCell_(yCell)
{

}


MorphGrid::~MorphGrid()
{
}

void MorphGrid::OnAddDragTarget(Vec2 oriDragPos)
{
	for (unsigned i = 1; i < xCell_; i++)
	{
		for (unsigned j = 1; j < yCell_; j++)
		{
			targetPoints_.push_back(GetPoint(i, j));
		}
	}
	targetPoints_.push_back(oriDragPos);
}

DragResult MorphGrid::TryDragRelease(const Vec2& nowPos)
{
	float minDis = FAR;
	Vec2 targetPos;
	for (auto& p : targetPoints_)
	{
		float tDis = (nowPos - p).Length();
		if (tDis < minDis)
		{
			minDis = tDis;
			targetPos = p;
		}
	}

	//???
	if (minDis != FAR)
	{
		return DragResult(true, targetPos);
	}
	else
	{
		return DragResult(false);
	}
}

//p1-p2
Vec2 MorphGrid::GetPoint(unsigned i, unsigned j) const
{
	Vec2 p1 = v4_ + (v1_ - v4_) / yCell_ * j;
	Vec2 p2 = v3_ + (v2_ - v3_) / yCell_ * j;
	return (p2 - p1)/xCell_*i + p1;
}

Vec2 MorphGrid::GetX1Dir() const
{
	return (v3_ - v4_).GetNormal();
}

Vec2 MorphGrid::GetX2Dir() const
{
	return (v2_ - v1_).GetNormal();
}

Vec2 MorphGrid::GetY1Dir() const
{
	return (v1_ - v4_).GetNormal();
}

Vec2 MorphGrid::GetY2Dir() const
{
	return (v2_ - v3_).GetNormal();
}

float MorphGrid::GetX1Length() const
{
	return (v3_ - v4_).Length();
}

float MorphGrid::GetX2Length() const
{
	return (v2_ - v1_).Length();
}

float MorphGrid::GetY1Length() const
{
	return (v1_ - v4_).Length();
}

float MorphGrid::GetY2Length() const
{
	return (v2_ - v3_).Length();
}