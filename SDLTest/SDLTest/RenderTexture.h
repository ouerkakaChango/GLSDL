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

	void UsePass(Pass* pass); 
	void UsePassOnlySelf(Pass* pass, bool bStartPass);

	~RenderTexture();

	GLuint renderTextureID_;
	GLuint frameBufferID_;
	GLuint colorRenderbuffer_;


private:

	Image* img_{nullptr};
	bool bFBOInitialized_{false};
	friend class DrawCall;
};

