#include "SceneShaderImage.h"

#include "RenderTexture.h"
#include "Material.h"
#include "Image.h"
#include "God.h"

SceneShaderImage::SceneShaderImage(Image* img, Material* material)
	:ShaderImage(img, material)
{
	rt_ = new RenderTexture(image_);
	clonedRT_ = new RenderTexture(image_);
}


SceneShaderImage::~SceneShaderImage()
{
}

void SceneShaderImage::GetDrawcall()
{
	if (bActive_)
	{
		auto& dcVec = GOD.postDrawcalls_;
		if (bUsePass_)
		{
			clonedRT_->SetTexture(rt_);
			clonedRT_->UsePass(pass_,true);

			material_->UpdateTextureParam("tex", rt_);
			//???
			material_->UpdateTextureParam("bluredTex", clonedRT_, 1);
			dcVec.push_back(dc_);
		}
		else
		{
			material_->UpdateTextureParam("tex", rt_);
			dcVec.push_back(dc_);
		}
	}
}