#pragma once
#include "ShaderImage.h"

#include "Utility.h"

class Button;

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
	void BindButton(Button* button);
	void SetFlipSound(const Path& flipSound);
	void SetChangeGroupSound(const Path& changeGroupSound);
protected:
	void CheckNextButtonStatus();
	void SetNextButtonActive(bool active);
	void BroadcastFlipEvent();

	MapVector<SDL_Surface*> texSurfaces_;
	SDL_Surface* nowSurface_{ nullptr };
	Button* nextButton_{ nullptr };
	Path flipSound_;
	Path changeGroupSound_;
	std::string nowGroup_{ "default" };
	unsigned nowInx_{ 0 };
	bool bUseFlipSound_{ false };
	bool bUseChangeGroupSound_{ false };
};

