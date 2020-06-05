#include "ShaderImage.h"

#include "Image.h"
#include "God.h"
#include "Material.h"
#include "DrawCall.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderTexture.h"
#include "Pass.h"

ShaderImage::ShaderImage(Image* img, Material* material):image_(img), material_(material)
{
	if (material_ == nullptr)
	{
		material_ = GOD.CloneDefaultMaterial();
	}

	dc_ = new DrawCall;
	dc_->SetMaterial(material_);

	//vb
	vb_ = new VertexBuffer;

	int w = GOD.gameConfig_.Get<int>("windowWidth");
	int h = GOD.gameConfig_.Get<int>("windowHeight");
	SDL_Rect rect = image_->GetSDLRect();
	Rect quad;
	quad.x = rect.x / (float)w * 2.0f - 1;
	quad.y = rect.y / (float)h * 2.0f - 1;
	quad.hw = rect.w / (float)w ;
	quad.hh = rect.h / (float)h ;
	vb_->InitQuad(quad);

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

void ShaderImage::GetDrawcall()
{
	if (bActive_)
	{
		if (name_ == "musicSImg")
		{
			int a = 1;
		}
		auto& dcVec = GOD.passiveDrawcalls_;
		if (bUsePass_)
		{
			//???
			passedRT_->SetTexture(rt_);
			passedRT_->UsePass(pass_);

			 material_->UpdateTextureParam("tex", rt_);
			//???
			material_->UpdateTextureParam("glowedTex", passedRT_->renderTextureID_,1);
			//material_->UpdateParam("bluredTex", IMG_Load("D:/HumanTree/Dante.png"));

			if (sceneRT_ != nullptr)
			{
				dc_->SetRenderTexture(sceneRT_);
			}

			dcVec.push_back(dc_);
		}
		else
		{
			if (sceneRT_ != nullptr)
			{
				dc_->SetRenderTexture(sceneRT_);
			}
			material_->UpdateParam("tex", image_->GetSurface());

			dcVec.push_back(dc_);
		}
	}
}

void ShaderImage::SetActive(bool active) 
{ 
	Drawable::SetActive(active);
}

void ShaderImage::Render()
{
	//??? (deprecated)
}

void ShaderImage::ChangeMaterial(Material* material)
{
	material->CloneType(material_);
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
		//???
		bUseEndPass_ = true;
		ResetRT();
		pass_ = pass;
		ChangeMaterial(endPass->GetMaterial());
		//???
		material_->name_ = "passmat";
	}
}

void ShaderImage::SetSceneRT(RenderTexture* sceneRT)
{
	Drawable::SetSceneRT(sceneRT);
	//???
	if (!bUseEndPass_)
	{
		Material* rtMaterial = new Material;
		sure(rtMaterial->CompileShader("D:/HumanTree/code/quadRT.vs", "D:/HumanTree/code/quadRT.fs"));
		ChangeMaterial(rtMaterial);
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

		testRT_ = new RenderTexture(GOD.testImg_);
	}
	else
	{
		//???
		abort();
	}
}