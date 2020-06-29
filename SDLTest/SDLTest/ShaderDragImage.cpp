#include "ShaderDragImage.h"

#include "God.h"
#include "Image.h"
#include "MorphGrid.h"
#include "Debug.h"

ShaderDragImage::ShaderDragImage(Image* img,
	Material* material, VBDrawType vbDrawType, TextureFilterType texFilterType)
	:ShaderImage(img,material,vbDrawType,texFilterType)
{
	GOD.BindEvent("LMB_Down", this);
	GOD.BindEvent("LMB_Up", this);

	EventGate gate1 = [&](Event* event)
	{
		LMB_Down* e = (LMB_Down*)event;
		if (InRect(e->x_, e->y_, image_->GetSDLRect()))
		{
			return true;
		}
		else
		{
			return false;
		}
	};
	BindEventGate("LMB_Down", gate1);

	EventHandler func1 = [&](Event* e)
	{
		LMB_Down* event = static_cast<LMB_Down*>(e);
		OnDragClick(event);
	};
	BindEventHandler("LMB_Down", func1);

	EventGate gate2 = [&](Event* event)
	{
		return bDraging_;
	};
	BindEventGate("LMB_Up", gate2);

	EventHandler func2 = [&](Event* e)
	{
		LMB_Up* event = static_cast<LMB_Up*>(e);
		OnDragRelease(event);
	};
	BindEventHandler("LMB_Up", func2);

	targetPoints_.push_back(image_->GetPosition());
	dragStart_ = targetPoints_[0];
}


ShaderDragImage::~ShaderDragImage()
{
}

void ShaderDragImage::OnDragClick(LMB_Down* event)
{
	LOG("DragClick");
	bDraging_ = true;
}

void ShaderDragImage::OnDragRelease(LMB_Up* event)
{
	LOG("DragRelease");
	bDraging_ = false;
}

void ShaderDragImage::AddDragTarget(const MorphGrid& grid)
{
	for (unsigned i = 1; i < grid.GetxCell(); i++)
	{
		for (unsigned j = 1; j < grid.GetyCell(); j++)
		{
			targetPoints_.push_back(grid.GetPoint(i, j));
		}
	}
}