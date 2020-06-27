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
	void InitByMorphGrid(const MorphGrid& grid, float width);
	std::vector<Quad> quads_;
protected:
};

class ShaderQuadGroup : public Drawable
{
public:
	ShaderQuadGroup(Image* img, const MorphGrid& grid, float width, Material* material=nullptr, VBDrawType vbDrawType = VB_Static, TextureFilterType texFilterType= TextureFilter_Linear);
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

};

