#include "ShaderPPT.h"

#include "SDL_image.h"
#include "Material.h"
#include "Image.h"

ShaderPPT::ShaderPPT(Image* initImg, Material* material, VBDrawType vbDrawType, TextureFilterType texFilterType)
	:ShaderImage(initImg, material, vbDrawType, texFilterType)
{
	nowSurface_ = initImg->GetSurface();
	texSurfaces_.push_back(nowSurface_);
	nowInx_ = 0;
}


ShaderPPT::~ShaderPPT()
{
}

void ShaderPPT::InsertPPT(const Path& texPath)
{
	texSurfaces_.push_back(IMG_Load(texPath.c_str()));
}

void ShaderPPT::Flip()
{
	if (nowInx_ + 1 >= texSurfaces_.size())
	{
		return;
	}
	nowInx_ += 1;
	nowSurface_ = texSurfaces_[nowInx_];
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
			////??? ��SceneShaderImg̫���Ƿ��д�ϲ���
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