#pragma once
#include "GLUtility.h"
class Material;
class VertexBuffer;
class IndexBuffer;
class DrawCall
{
public:
	DrawCall();
	~DrawCall();
	void Do();
	void SetMaterial(Material* const material);
	void SetVB(VertexBuffer* vb);
	void SetIB(IndexBuffer* ib);
	Material* material_;
protected:
	VertexBuffer* vb_;
	IndexBuffer* ib_;
};

