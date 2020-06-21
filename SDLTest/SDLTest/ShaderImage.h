#pragma once
#include "Drawable.h"

#include "Utility.h"
#include "GraphicDefs.h"

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
	ShaderImage(Image* img, Material* material=nullptr,VBDrawType drawType = VB_Static, TextureFilterType texFilterType = TextureFilter_Linear);	//material为空时，使用默认材质
	~ShaderImage();
	void SetActive(bool active) override;
	void GetDrawcall() override;
	void ChangeMaterial(Material* material);
	void UsePass(Pass* pass);
	void UsePass(Pass* pass, Pass* endPass);
	virtual void SetSceneRT(RenderTexture* sceneRT) override;

	void SetPosition(int x, int y);
	void SetImage(Image* img) { image_ = img; }
	Image*  GetImage() const{return image_;}

	Material* material_;
protected:
	void ResetRT();
	void CheckSetRTMaterial(MaterialBlendType blend);

	Image* image_;
	Pass* pass_;
	VertexBuffer* vb_;
	IndexBuffer* ib_;

	RenderTexture* rt_{nullptr};
	RenderTexture* passedRT_{ nullptr };

	VBDrawType drawType_{VB_Static};
	TextureFilterType texFilterType_{ TextureFilter_Linear };

	bool bUsePass_{false};
	bool bUseEndPass_{ false };
	bool bHasSetRTMaterial_{ false };
};

