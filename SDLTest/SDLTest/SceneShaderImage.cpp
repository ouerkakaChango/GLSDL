#include "SceneShaderImage.h"

#include "RenderTexture.h"
#include "Material.h"
#include "Image.h"
#include "God.h"

SceneShaderImage::SceneShaderImage(Image* img, Material* material)
	:ShaderImage(img, material)
{
	rt_ = new RenderTexture(image_);
	postRT_ = new RenderTexture(image_);
	auto postSwap = new RenderTexture(image_);
	postRT_->SetSwapRT(postSwap);
	SetDrawCallChannel(DrawCall_SceneColor);
}


SceneShaderImage::~SceneShaderImage()
{
}

void SceneShaderImage::GetDrawcall()
{
	if (bActive_)
	{
		if (bUsePass_)
		{
			{
				postRT_->SetTexture(rt_);
				postRT_->UsePass(pass_,true);
				
				material_->UpdateParam("tex", rt_);
				//??? 和SceneShaderImg太像，是否改写合并？
				material_->UpdateParam("bluredTex", postRT_);
			}
			CommitDrawCall();
		}
		else
		{
			material_->UpdateParam("tex", rt_);
			CommitDrawCall();
		}
	}
}