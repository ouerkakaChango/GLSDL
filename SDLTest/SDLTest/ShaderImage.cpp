#include "ShaderImage.h"

#include "Image.h"
#include "God.h"
#include "Material.h"
#include "DrawCall.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

ShaderImage::ShaderImage(Image* img, Material* material):image_(img), material_(material)
{
	if (material_ == nullptr)
	{
		material_ = GOD.CloneDefaultMaterial();
		//???
		material_->UpdateParam("tex", image_->GetSurface());
	}

	dc_ = new DrawCall;
	dc_->SetMaterial(material_);

	//vb
	vb_ = new VertexBuffer;
	//??? 会经常动的图片可能要改initQuad里static_draw?
	//??? 根据当前分辨率和image大小位置，计算屏幕坐标
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
		material_->UpdateParam("tex", image_->GetSurface());
		GOD.drawcalls_.push_back(dc_);
		//???
		if (name_ == "D:/HumanTree/test.png")
		{
			auto qqqa = 1;
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