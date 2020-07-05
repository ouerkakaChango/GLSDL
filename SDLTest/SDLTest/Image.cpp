#include "Image.h"

#include "SDL_image.h"
#include "GameConfig.h"
#include "God.h"

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
		//old draw
		texture_ = IMG_LoadTexture(renderer_, path.c_str());
		//gl draw
		surface_ = IMG_Load(path.c_str());
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

Rect Image::GetQuadRect()
{
	int w = GOD.windowW_;
	int h = GOD.windowH_;
	SDL_Rect rect = GetSDLRect();
	Rect quad;
	quad.x = rect.x / (float)w * 2.0f - 1;
	quad.y = rect.y / (float)h * 2.0f - 1;
	quad.hw = rect.w / (float)w ;
	quad.hh = rect.h / (float)h ;
	return quad;
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

SDL_Surface* Image::GetSurface()
{ 
	sure(surface_ !=nullptr);
	return surface_; 
}

void Image::SetSize(Vec2 size)
{
	width_ = size.x_;
	height_ = size.y_;
}