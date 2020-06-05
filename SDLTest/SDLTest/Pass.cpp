#include "Pass.h"

#include "Material.h"

void Pass::SetShader(const Path& vsPath, const Path& fsPath, MaterialBlendType blendType)
{
	bSelfEmpty_ = false;
	vsPath_ = vsPath;
	fsPath_ = fsPath;
	blendType_ = blendType;
	GetMaterial();
}

Material* Pass::GetMaterial()
{
	if (bSelfEmpty_){ return nullptr; }

	if (material_ == nullptr)
	{
		material_ = new Material;
		sure(material_->CompileShader(vsPath_, fsPath_));
		material_->SetBlendType(blendType_);
	}
	return material_;
}

void Pass::GetDoablePassVec(std::vector<Pass*>& vec)
{
	if (!bSelfEmpty_)
	{
		vec.push_back(this);
	}
	for (auto& child : children_)
	{
		child->GetDoablePassVec(vec);
	}
}