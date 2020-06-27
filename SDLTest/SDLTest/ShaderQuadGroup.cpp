#include "ShaderQuadGroup.h"

#include "God.h"
#include "DrawCall.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Image.h"
#include "Material.h"

void QuadGroup::InitByMorphGrid(const MorphGrid& grid, float width)
{
	//first verticle line quads
	Vec2 x1Dir = grid.GetX1Dir();
	Vec2 x2Dir = grid.GetX2Dir();
	for (unsigned i = 0; i <= grid.xCell_; i++)
	{
		Vec2 p1 = grid.GetPoint(i, 0);
		Vec2 p2 = grid.GetPoint(i, grid.yCell_);
		Quad verticleQuad;
		verticleQuad.VerticleLineQuad(p1, p2, x1Dir, x2Dir, width);
		quads_.push_back(verticleQuad);
	}

	Vec2 y1Dir = grid.GetY1Dir();
	Vec2 y2Dir = grid.GetY2Dir();

	for (unsigned j = 0; j <= grid.yCell_; j++)
	{
		Vec2 p1 = grid.GetPoint(0, j);
		Vec2 p2 = grid.GetPoint(grid.xCell_, j);
		Quad horiQuad;
		horiQuad.HoriLineQuad(p1, p2, y1Dir, y2Dir, width);
		quads_.push_back(horiQuad);
	}
	//???
	int i = 1;
}

ShaderQuadGroup::ShaderQuadGroup(Image* img, const MorphGrid& grid, float width, Material* material, VBDrawType vbDrawType, TextureFilterType texFilterType)
	:image_(img),
	material_(material),
	vbDrawType_(vbDrawType),
	texFilterType_(texFilterType)
{
	quadGroup_.InitByMorphGrid(grid, width);
	if (material_ == nullptr)
	{
		material_ = GOD.CloneDefaultMaterial();
	}

	dc_ = new DrawCall;
	//???
	dc_->name_ = "quadGroupDC";

	vb_ = new VertexBuffer;
	vb_->InitQuadGroup(quadGroup_, vbDrawType_);

	ib_ = new IndexBuffer;
	ib_->InitQuadGroup(vb_, quadGroup_);

	dc_->SetVB(vb_);
	dc_->SetIB(ib_);

	GOD.drawcallDrawables_.push_back(this);
}


ShaderQuadGroup::~ShaderQuadGroup()
{
}

void ShaderQuadGroup::GetDrawcall()
{
	if (bActive_)
	{
		{
			material_->UpdateParam("tex", image_->GetSurface());
			CommitDrawCall();
		}
	}
}

void ShaderQuadGroup::SetSceneRT(RenderTexture* sceneRT)
{
	sure(sceneRT != nullptr);
	Drawable::SetSceneRT(sceneRT);
	//if not use end pass,when set a sceneRT,means self should be use as an RT draw to sceneRT
	if (!bUseEndPass_)
	{
		CheckSetRTMaterial(material_->GetBlendType());
	}
}

void ShaderQuadGroup::CheckSetRTMaterial(MaterialBlendType blend)
{
	if (bHasSetRTMaterial_)
	{
		return;
	}
	Material* rtMaterial = GOD.defaultRTMaterial_->Clone();
	ChangeMaterial(rtMaterial);
	bHasSetRTMaterial_ = true;
}

void ShaderQuadGroup::ChangeMaterial(Material* material)
{
	material->CloneType(material_, texFilterType_);
	material_ = material;
	dc_->SetMaterial(material_);
}