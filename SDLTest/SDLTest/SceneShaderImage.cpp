#include "SceneShaderImage.h"

#include "RenderTexture.h"
#include "Material.h"
#include "Image.h"
#include "God.h"
#include "DrawCall.h"

SceneShaderImage::SceneShaderImage(Image* img, Material* material)
	:ShaderImage(img, material)
{
	rt_ = new RenderTexture(image_);
	postRT_ = new RenderTexture(image_);
	auto postSwap = new RenderTexture(image_);
	postRT_->SetSwapRT(postSwap);
	SetDrawCallChannel(DrawCall_SceneColor);

	preDC_ = new DrawCall;
	preDC_->SetVB(vb_);
	preDC_->SetIB(ib_);
	preDC_->SetMaterial(GOD.defaultRTMaterial_->Clone());
	preDC_->SetRenderTexture(rt_);
}


SceneShaderImage::~SceneShaderImage()
{
}

void SceneShaderImage::GetDrawcall()
{
	if (bActive_)
	{
		CommitPrePassive();
		if (bUsePass_)
		{
			{
				postRT_->SetTexture(rt_);
				postRT_->UsePass(pass_,true);
				
				material_->UpdateParam("tex", rt_);
				//??? ��SceneShaderImg̫���Ƿ��д�ϲ���
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

void SceneShaderImage::CommitPrePassive()
{
	preDC_->material_->UpdateParam("tex", image_->GetSurface());
	GOD.prePassiveDrawcalls_.push_back(preDC_);
}