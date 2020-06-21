#include "ShaderImage.h"

#include "Image.h"
#include "God.h"
#include "Material.h"
#include "DrawCall.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderTexture.h"
#include "Pass.h"

ShaderImage::ShaderImage(Image* img, Material* material, VBDrawType drawType, TextureFilterType texFilterType)
	:image_(img), 
	material_(material),
	drawType_(drawType),
	texFilterType_(texFilterType)
{
	if (material_ == nullptr)
	{
		material_ = GOD.CloneDefaultMaterial(texFilterType_);
	}

	dc_ = new DrawCall;
	dc_->SetMaterial(material_);

	//vb
	vb_ = new VertexBuffer;

	Rect quad = image_->GetQuadRect();
	vb_->InitQuad(quad, drawType_);

	//ib
	ib_ = new IndexBuffer;
	ib_->InitQuad(vb_->vao_);

	dc_->SetVB(vb_);
	dc_->SetIB(ib_);

	GOD.drawcallDrawables_.push_back(this);
}


ShaderImage::~ShaderImage()
{
}

void ShaderImage::SetPosition(int x, int y)
{
	image_->SetPosition(x, y);

	Rect quad = image_->GetQuadRect();
	vb_->SetQuad(quad);
}

void ShaderImage::GetDrawcall()
{
	if (bActive_)
	{
		//???
		if (name_ == "cursor")
		{
			int a = 1;
			dc_->name_ = "cursorDC";
		}
		if (name_ == "fadeBlack")
		{
			int b = 1;
		}
		if (bUsePass_)
		{
			passedRT_->SetTexture(rt_);
			passedRT_->UsePass(pass_);

			material_->UpdateParam("tex", rt_);
			//??? 和SceneShaderImg太像，是否改写合并？
			material_->UpdateParam("glowedTex", passedRT_);

			CommitDrawCall();
		}
		else
		{
			material_->UpdateParam("tex", image_->GetSurface());

			CommitDrawCall();
		}
	}
}

void ShaderImage::SetActive(bool active) 
{ 
	Drawable::SetActive(active);
}

void ShaderImage::ChangeMaterial(Material* material)
{
	material->CloneType(material_,texFilterType_);
	material_ = material;
	dc_->SetMaterial(material_);
}

void ShaderImage::UsePass(Pass* pass)
{
	if (pass == nullptr)
	{
		bUsePass_ = false;
		pass_ = nullptr;
	}
	else
	{
		bUsePass_ = true;
		pass_ = pass;
	}
}

void ShaderImage::UsePass(Pass* pass, Pass* endPass)
{
	if (pass == nullptr)
	{
		bUsePass_ = false;
		pass_ = nullptr;
	}
	else
	{
		bUsePass_ = true;
		bUseEndPass_ = true;
		ResetRT();
		pass_ = pass;
		ChangeMaterial(endPass->GetMaterial());
		//??? debug
		material_->name_ = "passmat";
	}
}

void ShaderImage::SetSceneRT(RenderTexture* sceneRT)
{
	sure(sceneRT != nullptr);
	Drawable::SetSceneRT(sceneRT);
	//if not use end pass,when set a sceneRT,means self should be use as an RT draw to sceneRT
	if (!bUseEndPass_)
	{
		CheckSetRTMaterial(material_->GetBlendType());
	}
}

void ShaderImage::ResetRT()
{
	if (rt_ == nullptr)
	{
		rt_ = new RenderTexture(image_);

		passedRT_ = new RenderTexture(image_);
		auto swapRT = new RenderTexture(image_);
		passedRT_->SetSwapRT(swapRT);
	}
	else
	{
		//??? unconsidered
		abort();
	}
}

void ShaderImage::CheckSetRTMaterial(MaterialBlendType blend)
{
	if (bHasSetRTMaterial_)
	{
		return;
	}
	if (blend == Blend_Opaque)
	{
		Material* rtMaterial = new Material;
		sure(rtMaterial->CompileShader("D:/HumanTree/code/quadRT.vs", "D:/HumanTree/code/quadRT.fs"));
		ChangeMaterial(rtMaterial);
	}
	else if (blend == Blend_Alpha)
	{
		//??? 这部分代码没测试过，可能能用于解决glow pass现在还是用的opaque的问题
		abort();
		Material* rtMaterial = new Material;
		sure(rtMaterial->CompileShader("D:/HumanTree/code/quadRT.vs", "D:/HumanTree/code/quadRTWithAlpha.fs"));
		ChangeMaterial(rtMaterial);
	}
	bHasSetRTMaterial_ = true;
}