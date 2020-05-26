#pragma once
//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
#include "Nameable.h"

#include "SDL_image.h"
#include "GLUtility.h"

class Image;
class Pass;
class Material;
class RenderTexture :
	public Nameable
{
public:
	RenderTexture();

	Material* UsePass(Pass* pass);
	SDL_Surface* GetSurface();
	~RenderTexture();

	GLuint renderTextureID_;
	GLuint frameBufferID_;
	GLuint colorRenderbuffer_;

private:
	SDL_Surface* surface_{nullptr};
	friend class DrawCall;
};

