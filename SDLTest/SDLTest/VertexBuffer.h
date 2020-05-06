#pragma once
#include "Nameable.h"
#include "Numeric.h"

#include "GLUtility.h"
class VertexBuffer : public Nameable
{
public:
	VertexBuffer();
	~VertexBuffer();

	void InitQuad(float scale);	//1Ϊ������Ļ
	void InitQuad(Rect quad);			//x,y��Ļ����ϵ��[-1,1]
	void InitQuad(
		float x1, float y1,
		float x2, float y2,
		float x3, float y3,
		float x4, float y4);	//x,y��Ļ����ϵ��[-1,1]

	GLuint bufferName_{0};
	GLuint vao_;
};

