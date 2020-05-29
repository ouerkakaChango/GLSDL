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
	glGenTextures(1, &renderTextureID_);
	glBindTexture(GL_TEXTURE_2D, renderTextureID_);

	//???
	int Mode = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, Mode, img_->GetWidth(), img_->GetHeight(), 0, Mode, GL_UNSIGNED_BYTE, img_->GetSurface()->pixels);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	{
		glGenFramebuffers(1, &frameBufferID_);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID_);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTextureID_, 0);
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
		sure(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

RenderTexture::RenderTexture(RenderTexture* src)
{
	sure(src != nullptr);

	img_ = src->img_;

	//???
	unsigned char   *srcPixels = (unsigned char*)malloc(img_->GetWidth()*img_->GetHeight() * 4);
	glBindTexture(GL_TEXTURE_2D, src->renderTextureID_);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, srcPixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &renderTextureID_);
	glBindTexture(GL_TEXTURE_2D, renderTextureID_);

	//???
	int Mode = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, Mode, img_->GetWidth(), img_->GetHeight(), 0, Mode, GL_UNSIGNED_BYTE, srcPixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	{
		glGenFramebuffers(1, &frameBufferID_);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID_);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTextureID_, 0);
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
		sure(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}


RenderTexture::~RenderTexture()
{
}



void RenderTexture::UsePass(Pass* pass, bool bPost)
{
	std::vector<Pass*> passes;
	pass->GetDoablePassVec(passes);
	for (unsigned i = 0; i < passes.size(); i++)
	{
		if (i == 0)
		{
			UsePassOnlySelf(passes[0], true, bPost);
		}
		else
		{
			UsePassOnlySelf(passes[i], false, bPost);
		}
	}
}

void RenderTexture::UsePassOnlySelf(Pass* pass, bool bStartPass, bool bPost)
{
	if (pass->SelfEmpty()) { return; }

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

	//dc
	DrawCall* rtDrawCall = new DrawCall;
	rtDrawCall->SetVB(rtvb);
	rtDrawCall->SetIB(rtib);

	Material* passMat = nullptr;
	if (bStartPass)
	{
		passMat = pass->GetMaterial()->Clone();
	}
	else
	{
		passMat = pass->GetMaterial();
	}
	sure(passMat != nullptr);

	rtDrawCall->SetMaterial(passMat);
	rtDrawCall->SetRenderTexture(this);
	
	if (bPost)
	{
		passMat->UpdateTextureParam("tex", renderTextureID_);
	}
	else
	{
		if (bStartPass)
		{
			passMat->UpdateParam("tex", img_->GetSurface());
		}
		else
		{
			passMat->UpdateTextureParam("tex", renderTextureID_);
		}
	}
	if (bPost)
	{
		GOD.postDrawcalls_.push_back(rtDrawCall);
	}
	else
	{
		GOD.passiveDrawcalls_.push_back(rtDrawCall);
	}
}

RenderTexture* RenderTexture::Clone()
{
	RenderTexture* re = new RenderTexture(this);
	return re;
}