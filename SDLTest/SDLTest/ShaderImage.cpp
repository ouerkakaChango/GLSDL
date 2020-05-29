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
		if (name_ == "testSImg")
		{
			auto v = 1;
		}
		auto& dcVec = GOD.passiveDrawcalls_;
		if (bUsePass_)
		{
			rt_ = new RenderTexture(image_);
			rt_->UsePass(pass_);

			material_->UpdateParam("tex", image_->GetSurface());
			//???
			material_->UpdateTextureParam("bluredTex", rt_, 1);
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
	//???
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
	}
	else
	{
		bUsePass_ = true;
		pass_ = pass;
	}
}

void ShaderImage::SetSceneRT(RenderTexture* sceneRT)
{
	Drawable::SetSceneRT(sceneRT);
	//???
	Material* rtMaterial = new Material;
	sure(rtMaterial->CompileShader("D:/HumanTree/code/quadRT.vs","D:/HumanTree/code/quadRT.fs"));
	ChangeMaterial(rtMaterial);
}