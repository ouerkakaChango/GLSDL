#pragma once
#include "Nameable.h"
#include "Tree.h"

#include "Utility.h"

class Image;
class RenderTexture;
class Material;

class Pass : public Nameable,public Tree<Pass>
{
public:
	void SetShader(const Path& vsPath, const Path& fsPath);
	bool SelfEmpty() { return bSelfEmpty_; }
	Material* GetMaterial();
protected:
	Material* material_{nullptr};
	bool bSelfEmpty_{ true };
	Path vsPath_, fsPath_;
};

