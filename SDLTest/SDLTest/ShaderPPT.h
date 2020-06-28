#pragma once
#include "ShaderImage.h"

#include "Utility.h"

class ShaderPPT : public ShaderImage
{
public:
	ShaderPPT(Image* initImg, Material* material = nullptr, VBDrawType vbDrawType = VB_Static, TextureFilterType texFilterType = TextureFilter_Linear);
	~ShaderPPT();
	void GetDrawcall() override;

	void InsertPPT(const Path& texPath);
	void Flip();

protected:
	std::vector<SDL_Surface*> texSurfaces_;
	unsigned nowInx_{ 0 };
	SDL_Surface* nowSurface_{ nullptr };
};

