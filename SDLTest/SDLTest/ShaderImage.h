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
	ShaderImage(Image* img, Material* material=nullptr);	//materialΪ��ʱ��ʹ��Ĭ�ϲ���
	~ShaderImage();
	void Render() override;
	void SetActive(bool active) override;
protected:
	Image* image_;
	//???
	DrawCall* dc_;
	VertexBuffer* vb_;
	IndexBuffer* ib_;
};

