#pragma once
#include "Drawable.h"

#include "Utility.h"
#include "Int.h"

class Image;
class ShaderImage;

class Cursor : public Drawable
{
public:
	//??? 25·Åµ½gameConfig?
	Cursor(const Path& imgPath="",int sizeX=25,int sizeY=25);

	//oldDraw
	void ChangeImage(const Path& imgPath);
	Image* GetImage() { return img_; }
	bool IsDefaultImg() { return bDefault_; }
	void SetDefaultImage();

	//glDraw
	ShaderImage* sImg_{ nullptr };

protected:
	//oldDraw
	Int<2> size_;
	Image* img_;
	bool bDefault_{ true };
	Path defaultImagePath_;
	StringMap<Image*> cacheImages_;


	void OnMouseMove(int x, int y);
};

