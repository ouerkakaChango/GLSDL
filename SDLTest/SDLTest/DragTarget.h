#pragma once
#include "Nameable.h"

#include "Math2D.h"
#include "Utility.h"

struct DragResult
{
	DragResult(bool canDrag=false, Vec2 pos=Vec2(0,0))
		:bCanDrag_(canDrag),
		position_(pos)
	{}
	bool bCanDrag_{ false };
	Vec2 position_;
};

class DragTarget : public Nameable
{
public:
	DragTarget();
	~DragTarget();

	virtual DragResult TryDragRelease(const Vec2& p)=0;
	virtual void OnAddDragTarget() {};

	std::vector<Vec2> targetPoints_;
};