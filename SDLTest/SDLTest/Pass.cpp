#include "Pass.h"

#include "Material.h"

void Pass::SetShader(const Path& vsPath, const Path& fsPath)
{
	bSelfEmpty_ = false;
	vsPath_ = vsPath;
	fsPath_ = fsPath;
}

Material* Pass::GetMaterial()
{
	if (bSelfEmpty_){ return nullptr; }

	if (material_ == nullptr)
	{
		material_ = new Material;
		material_->CompileShader(vsPath_, fsPath_);
	}
	return material_;
}