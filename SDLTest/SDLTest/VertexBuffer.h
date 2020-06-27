#pragma once
#include "Nameable.h"

#include "GraphicDefs.h"
#include "Numeric.h"
#include "GLUtility.h"

class QuadGroup;

class VertexBuffer : public Nameable
{
public:
	VertexBuffer();
	~VertexBuffer();

	void InitQuad(float scale);	//1为整个屏幕
	void InitQuad(Rect quad,VBDrawType drawType=VB_Static);			//x,y屏幕坐标系，[-1,1]
	void InitQuadGroup(const QuadGroup& quadGroup, VBDrawType drawType = VB_Static);

	void SetQuad(Rect quad);
	void InitQuad(
		float x1, float y1,
		float x2, float y2,
		float x3, float y3,
		float x4, float y4);	//x,y屏幕坐标系，[-1,1]
	GLuint bufferName_{0};

	GLuint vao_;
protected:
	GeometryType geomType_{ Geom_Normal };
	GLfloat* dataQuad_{nullptr};
	unsigned objNum_{ 0 };

	friend class DrawCall;
};

