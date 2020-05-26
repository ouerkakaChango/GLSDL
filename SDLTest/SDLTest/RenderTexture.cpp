#include "RenderTexture.h"

#include "Image.h"
#include "Material.h"


RenderTexture::RenderTexture()
{
	renderTextureID_ = 0;
	glGenTextures(1, &renderTextureID_);
	glBindTexture(GL_TEXTURE_2D, renderTextureID_);

	int Mode = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, Mode, 1600, 900, 0, Mode, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


RenderTexture::~RenderTexture()
{
}



Material* RenderTexture::UsePass(Pass* pass)
{
	frameBufferID_ = 0;
	glGenFramebuffers(1, &frameBufferID_);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID_);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTextureID_, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
	sure(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//???
	Material* mat = new Material;
	sure(mat->CompileShader("D:/HumanTree/code/quadRT.vs", "D:/HumanTree/code/horizenGaussianBlur.fs"));
	return mat;
}

SDL_Surface* RenderTexture::GetSurface()
{
	return surface_;
}