#include "SceneShaderImage.h"

#include "RenderTexture.h"
#include "Material.h"
#include "Image.h"
#include "God.h"

SceneShaderImage::SceneShaderImage(Image* img, Material* material)
	:ShaderImage(img, material)
{
	rt_ = new RenderTexture(image_);
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
			RenderTexture* clonedRT = rt_->Clone();
			clonedRT->UsePass(pass_,true);

			material_->UpdateTextureParam("tex", rt_);
			//???
			material_->UpdateTextureParam("bluredTex", clonedRT, 1);
			dcVec.push_back(dc_);
		}
		else
		{
			material_->UpdateTextureParam("tex", rt_);
			dcVec.push_back(dc_);
		}
	}
}