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

	
}


ShaderImage::~ShaderImage()
{

}

void ShaderImage::SetActive(bool active) 
{ 
	if (!bActive_ && active)
	{
		if (bUsePass_)
		{
			rt_ = new RenderTexture(image_);
			rt_->UsePass(pass_);

			material_->UpdateTextureParam("tex", rt_);
			GOD.drawcalls_.push_back(dc_);
		}
		else
		{
			material_->UpdateParam("tex", image_->GetSurface());
			GOD.drawcalls_.push_back(dc_);
		}
	}
	else if(bActive_&& !active)
	{
		auto& god = GOD;
		sure(STL_Remove(GOD.drawcalls_, dc_));
	}
	Drawable::SetActive(active);
}

void ShaderImage::Render()
{
	//???
}

void ShaderImage::ChangeMaterial(Material* material)
{
	material_ = material;
	dc_->SetMaterial(material_);
}

void ShaderImage::UsePass(Pass* pass)
{
	sure(pass != nullptr);
	bUsePass_ = true;
	pass_ = pass;
}