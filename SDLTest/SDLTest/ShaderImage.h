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
	XClass(ShaderImage)
public:
	ShaderImage(Image* img, Material* material=nullptr,VBDrawType drawType = VB_Static, TextureFilterType texFilterType = TextureFilter_Linear);	//materialΪ��ʱ��ʹ��Ĭ�ϲ���
	~ShaderImage();
	void SetActive(bool active) override;
	void GetDrawcall() override;
	void ChangeMaterial(Material* material);
	void UsePass(Pass* pass);
	virtual void SetSceneRT(RenderTexture* sceneRT) override;
	virtual void ChangeSize(Vec2 size) override;

	void SetPosition(int x, int y);
	void SetPosition(Vec2 pos);
	void SetImage(Image* img) { image_ = img; }
	Image*  GetImage() const{return image_;}
	void SetCustomDrawMaterial(Material* mat,const std::string& passedTexName);

	Material* material_;
protected:
	void PrepareRTForPass();
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
	bool bHasSetRTMaterial_{ false };

	bool bUseCustomDrawMat_{ false };
	std::string passedTexName_;
};

