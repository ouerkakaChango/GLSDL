#pragma once
//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
#include "Nameable.h"

#include "SDL_image.h"
#include "GLUtility.h"

class Image;
class Pass;
class Material;
class DrawCall;

class RenderTexture :
	public Nameable
{
public:
	RenderTexture(Image* img);
	RenderTexture(RenderTexture* src);
	~RenderTexture();

	void UsePass(Pass* pass,bool bPost=false); 
	void UsePassOnlySelf(Pass* pass, bool bStartPass, bool bPost = false);
	RenderTexture* Clone();

	GLuint renderTextureID_;
	GLuint frameBufferID_;
	GLuint colorRenderbuffer_;


private:

	Image* img_{nullptr};
	friend class DrawCall;
};

