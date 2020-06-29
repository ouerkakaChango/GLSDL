#pragma once
#include "ShaderImage.h"

#include "Events.h"
#include "Math2D.h"
#include "Utility.h"

class MorphGrid;

class ShaderDragImage : public ShaderImage
{
public:
	ShaderDragImage(Image* img,Material* material = nullptr, 
		VBDrawType vbDrawType = VB_Static, TextureFilterType texFilterType = TextureFilter_Linear);
	~ShaderDragImage();

	void OnDragClick(LMB_Down* event);
	void OnDragRelease(LMB_Up* event);
	void AddDragTarget(const MorphGrid& grid);	//no edge points
private:
	std::vector<Vec2> targetPoints_;
	Vec2 dragStart_{ 0 };
	bool bDraging_{ false };
};

