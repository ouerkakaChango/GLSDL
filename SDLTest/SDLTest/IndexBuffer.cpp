#include "IndexBuffer.h"

#include "VertexBuffer.h"
#include "ShaderQuadGroup.h"

IndexBuffer::IndexBuffer()
{
}


IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::InitQuad(GLuint vao)
{
	geomType_ = Geom_Quad;
	//IBO data
	GLuint indexData[] = { 0, 1, 3, 2 };

	glBindVertexArray(vao);

	//Create IBO
	glGenBuffers(1, &bufferName_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferName_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void IndexBuffer::InitQuadGroup(VertexBuffer* vb, const QuadGroup& quadGroup)
{
	geomType_ = Geom_QuadGroup;
	//IBO data
	//GLuint indexData[] = { 0, 1, 3, 2 };
	unsigned quadNum = quadGroup.quads_.size();
	quadData_ = new GLuint[6 * quadNum];
	for (unsigned i = 0; i < quadNum; i++)
	{
		unsigned ibInx = i * 6;
		unsigned vbInx = i * 4;
		quadData_[ibInx + 0] = 0 + vbInx;
		quadData_[ibInx + 1] = 1 + vbInx;
		quadData_[ibInx + 2] = 2 + vbInx;
		quadData_[ibInx + 3] = 2 + vbInx;
		quadData_[ibInx + 4] = 3 + vbInx;
		quadData_[ibInx + 5] = 0 + vbInx;
	}

	glBindVertexArray(vb->vao_);

	//Create IBO
	glGenBuffers(1, &bufferName_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferName_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * quadNum * sizeof(GLuint), quadData_, GL_STATIC_DRAW);
	glBindVertexArray(0);
}