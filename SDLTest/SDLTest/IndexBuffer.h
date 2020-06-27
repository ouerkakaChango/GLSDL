#pragma once
#include "GLUtility.h"
#include "GraphicDefs.h"

class VertexBuffer;
class QuadGroup;

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	void InitQuad(GLuint vao);
	void InitQuadGroup(VertexBuffer* vb, const QuadGroup& quadGroup);

	GLuint bufferName_{ 0 };
protected:
	GeometryType geomType_{Geom_Normal};
	GLuint* quadData_{ nullptr };
};

