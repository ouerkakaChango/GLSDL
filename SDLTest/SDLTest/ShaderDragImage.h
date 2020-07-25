#pragma once
#include "ShaderImage.h"

#include "Events.h"
#include "Math2D.h"
#include "Utility.h"

class MorphGrid;
class DragTarget;

class ShaderDragImage : public ShaderImage
{
public:
	ShaderDragImage(Image* img,Material* material = nullptr, 
		VBDrawType vbDrawType = VB_Static, TextureFilterType texFilterType = TextureFilter_Linear);
	~ShaderDragImage();

	void OnDragClick(LMB_Down* event);
	void OnDragRelease(LMB_Up* event);
	void OnDraging(Mouse_Move* event);
	void AddDragTarget(DragTarget* target);

	void SetDragReleaseSound(const Path& path);
protected:
	DragTarget* dragTarget_{nullptr};
	Path dragRleaseSound_;
	Vec2 oriDragPos_;
	bool bDraging_{ false };
	bool bUseDragRleaseSound_{ false };
};

