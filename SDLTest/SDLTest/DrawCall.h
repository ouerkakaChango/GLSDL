#pragma once
#include "Nameable.h"

#include "GLUtility.h"
class Material;
class VertexBuffer;
class IndexBuffer;
class RenderTexture;

class DrawCall : public Nameable
{
public:
	DrawCall();
	~DrawCall();
	void Do();
	void SimpleDoStart();
	void SimpleDoEnd();
	void SimpleDo();
	void SetMaterial(Material* const material);
	void SetVB(VertexBuffer* vb);
	void SetIB(IndexBuffer* ib);
	void SetRenderTexture(RenderTexture* rt);
	Material* material_;

protected:
	void BeginDo();
	void EndDo();

	VertexBuffer* vb_{nullptr};
	IndexBuffer* ib_{nullptr};
	RenderTexture* rt_{ nullptr };
	bool bDrawFrame_{ false };
	float renderWidth_, renderHeight_;
};

