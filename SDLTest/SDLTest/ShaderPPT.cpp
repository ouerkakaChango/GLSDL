#include "ShaderPPT.h"

#include "SDL_image.h"
#include "Material.h"
#include "Image.h"
#include "Button.h"
#include "God.h"
#include "BGMSystem.h"
#include "Events.h"

ShaderPPT::ShaderPPT(Image* initImg, Material* material, VBDrawType vbDrawType, TextureFilterType texFilterType)
	:ShaderImage(initImg, material, vbDrawType, texFilterType)
{
	nowSurface_ = initImg->GetSurface();
	texSurfaces_.map_["default"].push_back(nowSurface_);
	nowInx_ = 0;
}


ShaderPPT::~ShaderPPT()
{
}

void ShaderPPT::InsertPPT(const Path& texPath)
{
	texSurfaces_.map_["default"].push_back(IMG_Load(texPath.c_str()));
}

void ShaderPPT::InsertPPT(const std::string& groupName, const Path& texPath)
{
	texSurfaces_.map_[groupName].push_back(IMG_Load(texPath.c_str()));
}

void ShaderPPT::Flip()
{
	if (nowInx_ + 1 >= texSurfaces_.map_[nowGroup_].size())
	{
		return;
	}
	nowInx_ += 1;
	nowSurface_ = texSurfaces_.map_[nowGroup_][nowInx_];
	CheckNextButtonStatus();
	GOD.BroadCast(&PPT_PageFlip(nowGroup_,nowInx_, nowInx_ == texSurfaces_.map_[nowGroup_].size()-1));
	if (bUseFlipSound_)
	{
		GOD.bgmSystem_->PlayChunk(flipSound_);
	}
}

void ShaderPPT::GetDrawcall()
{
	if (bActive_)
	{
		if (bUsePass_)
		{
			//??? unconsidered yet
			abort();
		}
		else
		{
			material_->UpdateParam("tex", nowSurface_);
			CommitDrawCall();
		}
	}
}

void ShaderPPT::ChangeGroup(const std::string& groupName)
{
	if (texSurfaces_.HasKey(groupName))
	{
		nowGroup_ = groupName;
		nowInx_ = 0;
		nowSurface_ = texSurfaces_.map_[nowGroup_][nowInx_];
		CheckNextButtonStatus();
		if (bUseChangeGroupSound_)
		{
			GOD.bgmSystem_->PlayChunk(changeGroupSound_);
		}
	}
	else
	{
		abort();
	}
}

void ShaderPPT::BindButton(Button* button)
{
	nextButton_ = button;
}

void ShaderPPT::CheckNextButtonStatus()
{
	if (nowInx_ == texSurfaces_.map_[nowGroup_].size() - 1)
	{
		SetNextButtonActive(false);
	}
	else
	{
		SetNextButtonActive(true);
	}
}

void ShaderPPT::SetNextButtonActive(bool active)
{
	if (nextButton_ != nullptr)
	{
		nextButton_->SetActive(active);
	}
}

void ShaderPPT::SetFlipSound(const Path& flipSound)
{
	bUseFlipSound_ = true;
	flipSound_ = flipSound;
}

void ShaderPPT::SetChangeGroupSound(const Path& changeGroupSound)
{
	bUseChangeGroupSound_ = true;
	changeGroupSound_ = changeGroupSound;
}