#pragma once
#include "Drawable.h"

#include "Utility.h"
#include "Int.h"

class Image;
class ShaderImage;
class Material;

class Cursor : public Drawable
{
public:
	//??? 25�ŵ�gameConfig?
	Cursor(const Path& imgPath="",int sizeX=25,int sizeY=25);

	void SetActive(bool active) override;

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
	Material* defaultMat_{nullptr};
	bool bDefault_{ true };
	Path defaultImagePath_;
	StringMap<Image*> cacheImages_;


	void OnMouseMove(int x, int y);
};

