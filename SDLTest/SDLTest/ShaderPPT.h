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
	void InsertPPT(const std::string& groupName, const Path& texPath);
	void Flip();
	void ChangeGroup(const std::string& groupName);

protected:
	MapVector<SDL_Surface*> texSurfaces_;
	SDL_Surface* nowSurface_{ nullptr };
	std::string nowGroup_{ "default" };
	unsigned nowInx_{ 0 };
};

