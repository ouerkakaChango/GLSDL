#include "Image.h"

#include "SDL_image.h"
#include "GameConfig.h"

Image::Image(unsigned width, unsigned height):width_(width),height_(height), anchorType_(Anchor_Center)
{
}


Image::~Image()
{
	//!!! 释放 texture
}

bool Image::Load(const Path& path)
{
	//!!! 其他格式
	if(IsExtention(path,".bmp"))
	{
		SDL_Surface* surface;
		surface = SDL_LoadBMP(path.c_str());
		texture_ = SDL_CreateTextureFromSurface(renderer_, surface);
	}
	else if (IsExtention(path, ".png"))
	{
		texture_ = IMG_LoadTexture(renderer_, path.c_str());
	}
	return texture_ != nullptr;
}

bool Image::ReadFile(const Path& path)
{
	surface_ = IMG_Load(path.c_str());
	return true;
}

SDL_Rect Image::GetSDLRect()
{
	SDL_Rect re{0,0,0,0};
	if (anchorType_ == Anchor_Center)
	{
		re = { posx_, posy_, static_cast<int>(width_), static_cast<int>(height_) };
	}
	else if(anchorType_ == Anchor_LeftDown)
	{
		re = { posx_ - static_cast<int>(width_ / 2),posy_ - static_cast<int>(height_ / 2),static_cast<int>(width_),static_cast<int>(height_) };
		return re;
	}
	return re;
}

SDL_Rect Image::GetSDLRect(AnchorType type)
{
	SDL_Rect re{ 0,0,0,0 };
	if (type == Anchor_Center)
	{
		re = { posx_, posy_, static_cast<int>(width_), static_cast<int>(height_) };
	}
	else if (type == Anchor_LeftDown)
	{
		re = { posx_ - static_cast<int>(width_ / 2),posy_ - static_cast<int>(height_ / 2),static_cast<int>(width_),static_cast<int>(height_) };
		return re;
	}
	return re;
}

void Image::Render()
{
	if (texture_)
	{
		SDL_Rect picrect = GetSDLRect(Anchor_LeftDown);
		SDL_RenderCopy(renderer_, texture_, nullptr, &picrect);
	}
}

void Image::SetPosition(int x, int y)
{
	posx_ = x;
	posy_ = y;
}