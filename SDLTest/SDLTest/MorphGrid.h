#pragma once
#include "Math2D.h"

//p4 p3  ---> x1Dir
//p1 p2  ---> x2Dir
//y1Dir: p4 to p1
//y2Dir: p3 to p2
class MorphGrid
{
public:
	MorphGrid(Vec2 v1,Vec2 v2, Vec2 v3, Vec2 v4, int xCell, int yCell);
	~MorphGrid();

	Vec2 GetPoint(unsigned i, unsigned j) const;

	Vec2 GetX1Dir() const;
	Vec2 GetX2Dir() const;
	Vec2 GetY1Dir() const;
	Vec2 GetY2Dir() const;
protected:
	Vec2 v1_, v2_, v3_, v4_;
	unsigned xCell_{ 0 };
	unsigned yCell_{ 0 };

	friend class QuadGroup;
};

