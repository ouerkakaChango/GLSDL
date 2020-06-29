#pragma once
#include "Utility.h"
#include "Drawable.h"
#include "Math2D.h"

class GameConfig;

enum AnchorType
{
	Anchor_Center,
	Anchor_LeftDown,
};

class Image :public Drawable
{
public:
	Image(unsigned width, unsigned height);
	~Image();

	void Render() override;

	bool Load(const Path& path); //(deprecated)
	void SetPosition(int x, int y);
	Vec2 GetPosition() const { return Vec2(posx_, posy_); }
	unsigned GetWidth() { return width_; }
	unsigned GetHeight() { return height_; }
	SDL_Rect GetSDLRect();
	SDL_Rect GetSDLRect(AnchorType type);
	Rect GetQuadRect();

	//--- 新写法相关
	bool ReadFile(const Path& path);
	SDL_Surface* GetSurface();
	//___

	SDL_Texture* texture_;//(deprecated)

protected:
	SDL_Surface* surface_{nullptr};
	unsigned width_;
	unsigned height_;
	AnchorType anchorType_;
	int posx_;
	int posy_;
};

