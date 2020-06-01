#pragma once
#include "ShaderImage.h"
class RenderTexture;
class SceneShaderImage :
	public ShaderImage
{
public:
	SceneShaderImage(Image* img, Material* material = nullptr);
	~SceneShaderImage();
	void GetDrawcall() override;

	RenderTexture* GetRT() { return rt_; }
protected:
	RenderTexture* rt_;					//pre postRT
	RenderTexture* postRT_{nullptr};	//postRT
};

