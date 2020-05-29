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
	~RenderTexture();

	void UsePass(Pass* pass,bool bPost=false); 
	void UsePassOnlySelf(Pass* pass, bool bPost = false);
	void SetTexture(RenderTexture* src);

	GLuint renderTextureID_;
	GLuint frameBufferID_;
	GLuint colorRenderbuffer_;


private:

	Image* img_{nullptr};
	//???
	unsigned char  * srcPxiels_;
	bool copyInitialized_{ false };
	bool bEntryMatInitialized_{ false };
	Material* entryMaterial_{ nullptr };
	DrawCall* dc_;
	std::vector<Pass*> passes_;

	friend class DrawCall;
};

