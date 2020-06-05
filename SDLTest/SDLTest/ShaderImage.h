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
	void GetDrawcall() override;
	void ChangeMaterial(Material* material);
	void UsePass(Pass* pass);
	void UsePass(Pass* pass, Pass* endPass);
	virtual void SetSceneRT(RenderTexture* sceneRT) override;

	Material* material_;
protected:
	void ResetRT();

	Image* image_;
	Pass* pass_;
	DrawCall* dc_;
	VertexBuffer* vb_;
	IndexBuffer* ib_;

	//???
	RenderTexture* rt_{nullptr};
	RenderTexture* passedRT_{ nullptr };
	//???
	RenderTexture* testRT_{ nullptr };
	bool bUsePass_{false};
	bool bUseEndPass_{ false };
};

