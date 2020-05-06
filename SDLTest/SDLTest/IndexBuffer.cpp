#include "IndexBuffer.h"



IndexBuffer::IndexBuffer()
{
}


IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::InitQuad(GLuint vao)
{
	//IBO data
	GLuint indexData[] = { 0, 1, 3, 2 };

	glBindVertexArray(vao);

	//Create IBO
	glGenBuffers(1, &bufferName_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferName_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
	glBindVertexArray(0);
}