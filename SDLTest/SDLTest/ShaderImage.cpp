#include "ShaderImage.h"

#include "Image.h"
#include "God.h"
#include "Material.h"
#include "DrawCall.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderTexture.h"
#include "Pass.h"
#include "Debug.h"

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

void ShaderImage::SetPosition(Vec2 pos)
{
	SetPosition((int)pos.x_, (int)pos.y_);
}

void ShaderImage::GetDrawcall()
{
	//???
	if (name_ == "musicSImg")
	{
		int a = 1;
	}
	if (bActive_)
	{
		if (bUsePass_)
		{
			passedRT_->SetTexture(rt_);
			passedRT_->UsePass(pass_);

			if (name_ == "musicSImg")
			{
				rt_->name_ = "glowMusicRT";
			}

			if (!bUseCustomDrawMat_)
			{
				material_->UpdateTextureParam("tex", passedRT_->GetFinalTex());
			}
			else
			{
				material_->UpdateParam("tex", rt_);
				material_->UpdateTextureParam(passedTexName_, passedRT_->GetFinalTex());
				//???
				if (name_ == "musicSImg")
				{
					dc_->name_ = "musicDC";
				}
			}
		}
		else
		{
			material_->UpdateParam("tex", image_->GetSurface());
		}
		CommitDrawCall();
	}
}

void ShaderImage::SetActive(bool active) 
{ 
	Drawable::SetActive(active);
}

void ShaderImage::ChangeMaterial(Material* material)
{
	if (name_ == "musicSImg")
	{
		int i = 1;
	}
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
		PrepareRTForPass();
		//??? debug
		material_->name_ = "passmat";
	}
}

void ShaderImage::SetSceneRT(RenderTexture* sceneRT)
{
	sure(sceneRT != nullptr);
	Drawable::SetSceneRT(sceneRT);
	if (!bUseCustomDrawMat_)
	{
		CheckSetRTMaterial(material_->GetBlendType());
	}
}

void ShaderImage::PrepareRTForPass()
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
	Material* rtMaterial = GOD.defaultRTMaterial_->Clone();
	ChangeMaterial(rtMaterial);
	bHasSetRTMaterial_ = true;
}

void ShaderImage::ChangeSize(Vec2 size)
{
	//LOG2(size.x_,size.y_);

	image_->SetSize(size);
	Rect quad = image_->GetQuadRect();
	vb_->SetQuad(quad);
}

void ShaderImage::SetCustomDrawMaterial(Material* mat, const std::string& passedTexName)
{
	bUseCustomDrawMat_ = true;
	passedTexName_ = passedTexName;
	sure(mat != nullptr);
	material_ = mat;
	dc_->SetMaterial(material_);
}