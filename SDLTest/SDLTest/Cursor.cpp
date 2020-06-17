#include "Cursor.h"

#include "God.h"
#include "Image.h"
#include "Mouse_Move.h"

#include "ShaderImage.h"
#include "Material.h"

#include "Debug.h"

Cursor::Cursor(const Path& imgPath, int sizeX , int sizeY):size_(Int<2>(sizeX, sizeY))
{
	SetActive(true);
	if (!imgPath.empty())
	{
		SDL_ShowCursor(SDL_DISABLE);
		GOD.BindEvent("Mouse_Move", this);
		img_ = new Image(size_[0], size_[1]);
		if (img_->Load(imgPath))
		{
			defaultImagePath_ = imgPath;
			cacheImages_.map_[imgPath] = img_;
			EventHandler moveMoveHandler = [&](Event* event)
			{
				Mouse_Move* moveEvent = static_cast<Mouse_Move*>(event);
				OnMouseMove(moveEvent->x_, moveEvent->y_);
			};
			BindEventHandler("Mouse_Move", moveMoveHandler);
			GOD.AddPostDrawable(img_);
		}
	}
	//glDraw
	sImg_ = new ShaderImage(GOD.testImg_);
	sImg_->material_->SetBlendType(Blend_Alpha);
	sImg_->SetDrawCallChannel(DrawCall_AfterPost);
	if (GOD.bOldDraw_)
	{
		sImg_->SetActive(false);
	}
}

void Cursor::OnMouseMove(int x, int y)
{
	if (GOD.bOldDraw_)
	{
		img_->SetPosition(x, y);
	}
}

void Cursor::ChangeImage(const Path& imgPath)
{
	if (GOD.bOldDraw_)
	{
		auto temp = img_;
		if (!imgPath.empty())
		{
			Image** result = nullptr;
			if (cacheImages_.Find(imgPath, result))
			{
				if (result != nullptr)
				{
					img_ = *result;
				}
			}
			else
			{
				img_ = new Image(size_[0], size_[1]);
				img_->Load(imgPath);
				cacheImages_.map_[imgPath] = img_;
			}
			GOD.ChangePostDrawable(temp, img_);
			bDefault_ = false;
		}
	}
}

void Cursor::SetDefaultImage()
{
	if (GOD.bOldDraw_)
	{
		ChangeImage(defaultImagePath_);
		bDefault_ = true;
	}
}