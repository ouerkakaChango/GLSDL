#pragma once
#include "Drawable.h"

#include "Utility.h"

class Material;
class Image;
class DrawCall;
class VertexBuffer;
class IndexBuffer;
class Pass;
class RenderTexture;

class ShaderImage : public Drawable
{
public:
	ShaderImage(Image* img, Material* material=nullptr);	//material为空时，使用默认材质
	~ShaderImage();
	void Render() override;
	void SetActive(bool active) override;
	void ChangeMaterial(Material* material);
	void UsePass(Pass* pass);
protected:
	Image* image_;
	Material* material_;
	Pass* pass_;
	DrawCall* dc_;
	VertexBuffer* vb_;
	IndexBuffer* ib_;

	//???
	RenderTexture* rt_{nullptr};
	bool bUsePass_{false};
};

