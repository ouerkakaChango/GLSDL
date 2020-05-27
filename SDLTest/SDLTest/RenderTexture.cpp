#include "RenderTexture.h"

#include "Image.h"
#include "Material.h"
#include "DrawCall.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "God.h"
#include "Pass.h"

RenderTexture::RenderTexture(Image* img):img_(img)
{
	renderTextureID_ = 0;
	glGenTextures(1, &renderTextureID_);
	glBindTexture(GL_TEXTURE_2D, renderTextureID_);

	int Mode = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, Mode, img_->GetWidth(), img_->GetHeight(), 0, Mode, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


RenderTexture::~RenderTexture()
{
}



void RenderTexture::UsePass(Pass* pass)
{
	//???
}

void RenderTexture::UsePassOnlySelf(Pass* pass)
{
	if (pass->SelfEmpty()) { return; }

	DrawCall* rtDrawCall = new DrawCall;
	//vb
	auto rtvb = new VertexBuffer;
	int w = GOD.gameConfig_.Get<int>("windowWidth");
	int h = GOD.gameConfig_.Get<int>("windowHeight");
	SDL_Rect rect = img_->GetSDLRect();
	Rect quad;
	quad.x = rect.x / (float)w * 2.0f - 1;
	quad.y = rect.y / (float)h * 2.0f - 1;
	quad.hw = rect.w / (float)w;
	quad.hh = rect.h / (float)h;
	rtvb->InitQuad(quad);

	//ib
	auto rtib = new IndexBuffer;
	rtib->InitQuad(rtvb->vao_);

	rtDrawCall->SetVB(rtvb);
	rtDrawCall->SetIB(rtib);

	//---
	glGenFramebuffers(1, &frameBufferID_);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID_);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTextureID_, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
	sure(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//___

	Material* passMat = pass->GetMaterial();
	sure(passMat != nullptr);

	rtDrawCall->SetMaterial(passMat);
	rtDrawCall->SetDrawFrame(true);
	rtDrawCall->rt_ = this;
	
	passMat->UpdateParam("tex", img_->GetSurface());
	GOD.drawcalls_.push_back(rtDrawCall);
}