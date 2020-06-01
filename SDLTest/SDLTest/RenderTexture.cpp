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
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	{
		glGenFramebuffers(1, &frameBufferID_);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID_);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTextureID_, 0);
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
		sure(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//???
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
	dc_ = new DrawCall;
	dc_->SetVB(rtvb);
	dc_->SetIB(rtib);
}

void RenderTexture::SetTexture(RenderTexture* src)
{
	//???
	int Mode = GL_RGBA;

	//if(false)
	//{//copy from ori,stupid version
	//	if (!copyInitialized_)
	//	{
	//		srcPxiels_ = (unsigned char*)malloc(img_->GetWidth()*img_->GetHeight() * 4);
	//		copyInitialized_ = true;
	//	}
	//	glBindTexture(GL_TEXTURE_2D, src->renderTextureID_);
	//	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, srcPxiels_);
	//	glBindTexture(GL_TEXTURE_2D, renderTextureID_);
	//	glTexImage2D(GL_TEXTURE_2D, 0, Mode, img_->GetWidth(), img_->GetHeight(), 0, Mode, GL_UNSIGNED_BYTE, srcPxiels_);
	//}

	//GL4.3 required
	glBindTexture(GL_TEXTURE_2D, renderTextureID_);
	glTexImage2D(GL_TEXTURE_2D, 0, Mode, img_->GetWidth(), img_->GetHeight(), 0, Mode, GL_UNSIGNED_BYTE, NULL);
	glCopyImageSubData(src->renderTextureID_, GL_TEXTURE_2D, 0, 0, 0, 0,
			renderTextureID_, GL_TEXTURE_2D, 0, 0, 0, 0,
			img_->GetWidth(), img_->GetHeight(), 1);
}

RenderTexture::~RenderTexture()
{
}



void RenderTexture::UsePass(Pass* pass, bool bPost)
{
	if (passes_.empty())
	{
		pass->GetDoablePassVec(passes_);
	}
	for (unsigned i = 0; i < passes_.size(); i++)
	{
		UsePassOnlySelf(passes_[i], bPost);
	}
}

void RenderTexture::UsePassOnlySelf(Pass* pass, bool bPost)
{
	if (pass->SelfEmpty()) { return; }

	Material* passMat = pass->GetMaterial();
	sure(passMat != nullptr);
	//??? 现在共用一个drawCall，多次call它
	dc_->SetMaterial(passMat);
	dc_->SetRenderTexture(this);
	
	passMat->UpdateTextureParam("tex", renderTextureID_);
	if (bPost)
	{
		GOD.postDrawcalls_.push_back(dc_);
	}
	else
	{
		GOD.passiveDrawcalls_.push_back(dc_);
	}
}