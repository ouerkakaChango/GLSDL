#pragma once
#include "GLUtility.h"
class Material;
class VertexBuffer;
class IndexBuffer;
//???
class RenderTexture;

class DrawCall
{
public:
	DrawCall();
	~DrawCall();
	void Do();
	void SetMaterial(Material* const material);
	void SetVB(VertexBuffer* vb);
	void SetIB(IndexBuffer* ib);
	void SetDrawFrame(bool drawFrame) { bDrawFrame_ = drawFrame; }
	Material* material_;

	//???
	RenderTexture* rt_;
protected:
	void BeginDo();
	void EndDo();

	VertexBuffer* vb_;
	IndexBuffer* ib_;
	bool bDrawFrame_{ false };
};

