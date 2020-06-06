#pragma once
//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
#include "Nameable.h"

#include "SDL_image.h"
#include "GLUtility.h"

class Image;
class Pass;
class Material;
class VertexBuffer;
class IndexBuffer;
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

	void SetSwapRT(RenderTexture* swapRT);
	GLuint GetFinalTex();
	float GetWidth();
	float GetHeight();

	GLuint renderTextureID_;
	GLuint frameBufferID_;
	GLuint colorRenderbuffer_;


private:
	void InitSwapables(Pass* pass);

	Image* img_{nullptr};
	std::vector<Pass*> passes_;
	//???
	unsigned char  * srcPxiels_;
	bool copyInitialized_{ false };
	//???
	bool swapFlag_{false};
	RenderTexture* swapRT_;
	VertexBuffer* vb_;
	IndexBuffer* ib_;
	DrawCall *selfDC_{nullptr}, *swapDC_;
	Material *selfDrawMat_, *swapDrawMat_;


	friend class DrawCall;
};

