#include "ShaderImage.h"

#include "Image.h"
#include "God.h"
#include "Material.h"
#include "DrawCall.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

ShaderImage::ShaderImage(Image* img, Material* material):image_(img)
{
	dc_ = new DrawCall;
	dc_->SetMaterial(material);

	//vb
	vb_ = new VertexBuffer;
	//??? �ᾭ������ͼƬ����Ҫ��initQuad��static_draw?
	//??? ���ݵ�ǰ�ֱ��ʺ�image��Сλ�ã�������Ļ����
	int w = GOD.gameConfig_.Get<int>("windowWidth");
	int h = GOD.gameConfig_.Get<int>("windowHeight");
	SDL_Rect rect = img->GetSDLRect();
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
		GOD.drawcalls_.push_back(dc_);
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