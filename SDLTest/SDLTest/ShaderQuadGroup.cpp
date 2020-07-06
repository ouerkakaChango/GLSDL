#include "ShaderQuadGroup.h"

#include "God.h"
#include "DrawCall.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Image.h"
#include "Material.h"
#include "RenderTexture.h"

void QuadGroup::InitByMorphGrid(const MorphGrid& grid, float width, float xScale, float yScale, bool bNeedEdgeQuad)
{
	//first verticle line quads
	Vec2 x1Dir = grid.GetX1Dir();
	Vec2 x2Dir = grid.GetX2Dir();
	float scaledW1 = grid.GetX1Length() / GOD.windowW_ *width * xScale;
	float scaledW2 = grid.GetX2Length() / GOD.windowW_ *width * xScale;
	for (unsigned i = 0; i <= grid.xCell_; i++)
	{
		if (!bNeedEdgeQuad && (i == 0 || i == grid.xCell_))
		{
			continue;
		}
		Vec2 p1 = grid.GetPoint(i, 0);
		Vec2 p2 = grid.GetPoint(i, grid.yCell_);
		Quad verticleQuad;
		verticleQuad.VerticleLineQuad(p1, p2, x1Dir, x2Dir, scaledW1, scaledW2);
		quads_.push_back(verticleQuad);
	}

	Vec2 y1Dir = grid.GetY1Dir();
	Vec2 y2Dir = grid.GetY2Dir();
	float scaledH1 = grid.GetY1Length() / GOD.windowH_ *width * yScale;
	float scaledH2 = grid.GetY2Length() / GOD.windowH_ *width * yScale;
	for (unsigned j = 0; j <= grid.yCell_; j++)
	{
		if (!bNeedEdgeQuad && (j == 0 || j == grid.yCell_))
		{
			continue;
		}
		Vec2 p1 = grid.GetPoint(0, j);
		Vec2 p2 = grid.GetPoint(grid.xCell_, j);
		Quad horiQuad;
		horiQuad.HoriLineQuad(p1, p2, y1Dir, y2Dir, scaledH1, scaledH2);
		quads_.push_back(horiQuad);
	}
}

ShaderQuadGroup::ShaderQuadGroup(Image* img, const MorphGrid& grid, float baseWidth, 
	float xScale,float yScale, bool bNeedEdgeQuad, 
	Material* material, VBDrawType vbDrawType, TextureFilterType texFilterType)
	:image_(img),
	bNeedEdgeQuad_(bNeedEdgeQuad),
	material_(material),
	vbDrawType_(vbDrawType),
	texFilterType_(texFilterType)
{
	quadGroup_.InitByMorphGrid(grid, baseWidth, xScale, yScale, bNeedEdgeQuad);
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
			if (bUsePass_)
			{
				passedRT_->SetTexture(rt_);
				passedRT_->UsePass(pass_);

				if (!bUseCustomDrawMat_)
				{
					material_->UpdateTextureParam("tex", passedRT_->GetFinalTex());
				}
				else
				{
					material_->UpdateParam("tex", rt_);
					material_->UpdateTextureParam(passedTexName_, passedRT_->GetFinalTex());
				}
			}
			else
			{
				material_->UpdateParam("tex", image_->GetSurface());
			}
			CommitDrawCall();
		}
	}
}

void ShaderQuadGroup::SetSceneRT(RenderTexture* sceneRT)
{
	sure(sceneRT != nullptr);
	Drawable::SetSceneRT(sceneRT);
	//if not use end pass,when set a sceneRT,means self should be use as an RT draw to sceneRT
	if (!bUseCustomDrawMat_)
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

void ShaderQuadGroup::UsePass(Pass* pass)
{
	if (pass == nullptr)
	{
		bUsePass_ = false;
		pass_ = nullptr;
	}
	else
	{
		bUsePass_ = true;
		pass_ = pass;
		PrepareRTForPass();
	}
}

void ShaderQuadGroup::PrepareRTForPass()
{
	if (rt_ == nullptr)
	{
		rt_ = new RenderTexture(image_);

		passedRT_ = new RenderTexture(image_);
		auto swapRT = new RenderTexture(image_);
		passedRT_->SetSwapRT(swapRT);
	}
	else
	{
		//??? unconsidered
		abort();
	}
}

void ShaderQuadGroup::SetCustomDrawMaterial(Material* mat, const std::string& passedTexName)
{
	bUseCustomDrawMat_ = true;
	passedTexName_ = passedTexName;
	sure(mat != nullptr);
	material_ = mat;
	dc_->SetMaterial(material_);
}