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
	if (GL_OUT_OF_MEMORY == glGetError())
	{
		abort();
	}
	glBindTexture(GL_TEXTURE_2D, renderTextureID_);

	//???
	int Mode = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, Mode, img_->GetWidth(), img_->GetHeight(), 0, Mode, GL_UNSIGNED_BYTE, img_->GetSurface()->pixels);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
	{
		glGenFramebuffers(1, &frameBufferID_);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID_);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTextureID_, 0);
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
		sure(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//由于RT都用于FBO，所以为屏幕大小
	//vb
	vb_ = new VertexBuffer;
	int w = GOD.gameConfig_.Get<int>("windowWidth");
	int h = GOD.gameConfig_.Get<int>("windowHeight");
	SDL_Rect rect = img_->GetSDLRect();
	Rect quad;
	quad.x = 0;
	quad.y = 0;
	quad.hw = 1;
	quad.hh = 1;
	vb_->InitQuad(quad);

	//ib
	ib_ = new IndexBuffer;
	ib_->InitQuad(vb_->vao_);

}

void RenderTexture::SetTexture(RenderTexture* src)
{
	//???
	int Mode = GL_RGBA;

	//if(false)
	{//copy from ori,stupid version
		//if (!copyInitialized_)
		//{
		//	srcPxiels_ = (unsigned char*)malloc(img_->GetWidth()*img_->GetHeight() * 4);
		//	copyInitialized_ = true;
		//}
		//glBindTexture(GL_TEXTURE_2D, src->renderTextureID_);
		//glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, srcPxiels_);
		//glBindTexture(GL_TEXTURE_2D, renderTextureID_);
		//glTexImage2D(GL_TEXTURE_2D, 0, Mode, img_->GetWidth(), img_->GetHeight(), 0, Mode, GL_UNSIGNED_BYTE, srcPxiels_);
	}

	//GL4.3 required
	//glBindTexture(GL_TEXTURE_2D, renderTextureID_);
	//glTexImage2D(GL_TEXTURE_2D, 0, Mode, img_->GetWidth(), img_->GetHeight(), 0, Mode, GL_UNSIGNED_BYTE, NULL);
	//???
	glCopyImageSubDataNV(src->renderTextureID_, GL_TEXTURE_2D, 0, 0, 0, 0,
			renderTextureID_, GL_TEXTURE_2D, 0, 0, 0, 0,
			img_->GetWidth(), img_->GetHeight(),1);
}

RenderTexture::~RenderTexture()
{
}



void RenderTexture::UsePass(Pass* pass, bool bPost)
{
	swapFlag_ = false;
	if (passes_.empty())
	{
		pass->GetDoablePassVec(passes_);
	}
	// initalize dc,mat for swap
	if (selfDC_ == nullptr)
	{
		//???
		InitSwapables(passes_[0]);
	}
	for (unsigned i = 0; i < passes_.size(); i++)
	{
		UsePassOnlySelf(passes_[i], bPost);
	}
}

void RenderTexture::InitSwapables(Pass* pass)
{
	selfDC_ = new DrawCall;
	swapDC_ = new DrawCall;
	//draw to swap
	selfDrawMat_ = pass->GetMaterial()->Clone();
	selfDrawMat_->UpdateParam("tex", this);
	//draw to self
	swapDrawMat_ = selfDrawMat_->Clone();
	swapDrawMat_->UpdateParam("tex", swapRT_);

	selfDC_->SetVB(vb_);
	selfDC_->SetIB(ib_);
	selfDC_->SetMaterial(selfDrawMat_);
	selfDC_->SetRenderTexture(swapRT_);

	swapDC_->SetVB(vb_);
	swapDC_->SetIB(ib_);
	swapDC_->SetMaterial(swapDrawMat_);
	swapDC_->SetRenderTexture(this);
}

void RenderTexture::UsePassOnlySelf(Pass* pass, bool bPost)
{
	//self as parameter,draw to swap
	if (!swapFlag_)
	{
		if (pass->SelfEmpty()) { return; }
		if (bPost)
		{
			GOD.postDrawcalls_.push_back(selfDC_);
		}
		else
		{
			//???
			selfDC_->name_ = "glowDC";
			GOD.passiveDrawcalls_.push_back(selfDC_);
		}
	}
	//swap as parameter,draw to self
	else
	{
		if (pass->SelfEmpty()) { return; }
		if (bPost)
		{
			GOD.postDrawcalls_.push_back(swapDC_);
		}
		else
		{
			//???
			swapDC_->name_ = "glowDC";
			GOD.passiveDrawcalls_.push_back(swapDC_);
		}
	}

	swapFlag_ = !swapFlag_;
}

void RenderTexture::SetSwapRT(RenderTexture* swapRT)
{
	swapRT_ = swapRT;
}

GLuint RenderTexture::GetFinalTex()
{
	return static_cast<int>(passes_.size())%2==0 ? renderTextureID_ : swapRT_->renderTextureID_;
}

float RenderTexture::GetWidth()
{
	sure(img_ != nullptr);
	return img_->GetWidth();
}

float RenderTexture::GetHeight()
{
	sure(img_ != nullptr);
	return img_->GetHeight();
}