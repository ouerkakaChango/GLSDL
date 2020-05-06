#pragma once
#include "Drawable.h"

#include "Utility.h"

class Material;
class Image;
class DrawCall;
class VertexBuffer;
class IndexBuffer;

class ShaderImage : public Drawable
{
public:
	ShaderImage(Image* img, Material* material);
	~ShaderImage();
	void Render() override;
protected:
	Image* image_;
	//???
	DrawCall* dc_;
	VertexBuffer* vb_;
	IndexBuffer* ib_;
};

