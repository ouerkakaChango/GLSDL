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
	ShaderImage(Image* img, Material* material=nullptr,VBDrawType drawType = VB_Static);	//materialΪ��ʱ��ʹ��Ĭ�ϲ���
	~ShaderImage();
	void SetActive(bool active) override;
	void GetDrawcall() override;
	void ChangeMaterial(Material* material);
	void UsePass(Pass* pass);
	void UsePass(Pass* pass, Pass* endPass);
	virtual void SetSceneRT(RenderTexture* sceneRT) override;

	void SetPosition(int x, int y);

	Material* material_;
protected:
	void ResetRT();

	Image* image_;
	Pass* pass_;
	VertexBuffer* vb_;
	IndexBuffer* ib_;

	RenderTexture* rt_{nullptr};
	RenderTexture* passedRT_{ nullptr };

	VBDrawType drawType_{VB_Static};

	bool bUsePass_{false};
	bool bUseEndPass_{ false };
};

