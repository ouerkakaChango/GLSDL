#pragma once
#include "Object.h"

#include "Utility.h"
#include "Int.h"

class Image;

class Cursor : public Object
{
public:
	Cursor(const Path& imgPath="",int sizeX=25,int sizeY=25);

	void ChangeImage(const Path& imgPath);
	Image* GetImage() { return img_; }
	bool IsDefaultImg() { return bDefault_; }
	void SetDefaultImage();
protected:
	Int<2> size_;
	Image* img_;
	bool bDefault_{ true };
	Path defaultImagePath_;
	StringMap<Image*> cacheImages_;

	void OnMouseMove(int x, int y);
};

