#include "ShaderPPT.h"

#include "SDL_image.h"
#include "Material.h"
#include "Image.h"

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
}

void ShaderPPT::GetDrawcall()
{
	if (bActive_)
	{
		if (bUsePass_)
		{
			//??? unconsidered yet
			abort();
			//passedRT_->SetTexture(rt_);
			//passedRT_->UsePass(pass_);
			//
			//material_->UpdateParam("tex", rt_);
			////??? 和SceneShaderImg太像，是否改写合并？
			//material_->UpdateParam("glowedTex", passedRT_);
			//
			//CommitDrawCall();
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
	}
	else
	{
		abort();
	}
}