#pragma once
#include "Drawable.h"

#include "Utility.h"
#include "MorphGrid.h"

class Material;
class Image;
class VertexBuffer;
class IndexBuffer;

class QuadGroup
{
public:
	void InitByMorphGrid(const MorphGrid& grid, float width, float xScale, float yScale, bool bNeedEdgeQuad);
	std::vector<Quad> quads_;
protected:
};

class ShaderQuadGroup : public Drawable
{
public:
	ShaderQuadGroup(Image* img, const MorphGrid& grid, float baseWidth,
		float xScale=1.0f, float yScale=1.0f, bool bNeedEdgeQuad = true, 
		Material* material=nullptr, VBDrawType vbDrawType = VB_Static, TextureFilterType texFilterType= TextureFilter_Linear);
	~ShaderQuadGroup();

	void GetDrawcall() override;
	void SetSceneRT(RenderTexture* sceneRT) override;

	void ChangeMaterial(Material* material);

	Material* material_{nullptr};
	QuadGroup quadGroup_;

protected:
	void CheckSetRTMaterial(MaterialBlendType blend);

	VertexBuffer* vb_;
	IndexBuffer* ib_;
	Image* image_{nullptr};

	TextureFilterType texFilterType_;
	VBDrawType vbDrawType_;
	bool bUseEndPass_{ false };
	bool bHasSetRTMaterial_{ false };
	bool bNeedEdgeQuad_{ true };

};

