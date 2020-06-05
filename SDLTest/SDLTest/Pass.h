#pragma once
#include "Nameable.h"
#include "Tree.h"

#include "Material.h"
#include "Utility.h"

class Image;
class RenderTexture;

class Pass : public Nameable,public Tree<Pass>
{
public:
	void SetShader(const Path& vsPath, const Path& fsPath, MaterialBlendType blendType = Blend_Opaque);
	bool SelfEmpty() { return bSelfEmpty_; }
	Material* GetMaterial();
	void GetDoablePassVec(std::vector<Pass*>& vec);
protected:
	Material* material_{nullptr};
	MaterialBlendType blendType_{ Blend_Opaque };
	bool bSelfEmpty_{ true };
	Path vsPath_, fsPath_;
};

