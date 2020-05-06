#include "VertexBuffer.h"



VertexBuffer::VertexBuffer()
{
}


VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::InitQuad(float scale)
{
	//VBO data
	GLfloat vertexData[] =
	{
		-scale, -scale, 0.0f,1.0f,
		 scale, -scale, 1.0f,1.0f,
		 scale,  scale, 1.0f,0.0f,
		-scale,  scale, 0.0f,0.0f
	};

	//VAO
	glGenVertexArrays(1, &vao_);	//对vao赋值
	glBindVertexArray(vao_);

	//Create VBO
	glGenBuffers(1, &bufferName_);
	glBindBuffer(GL_ARRAY_BUFFER, bufferName_);
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
	// Make sure the VAO is not changed from outside code
	glBindVertexArray(0);
}

void VertexBuffer::InitQuad(Rect quad)
{
	//VBO data
	GLfloat vertexData[] =
	{
		quad.x-quad.hw, quad.y-quad.hh, 0.0f,1.0f,
		quad.x+quad.hw, quad.y-quad.hh, 1.0f,1.0f,
		quad.x+quad.hw, quad.y+quad.hh, 1.0f,0.0f,
		quad.x-quad.hw, quad.y+quad.hh, 0.0f,0.0f
	};

	//VAO
	glGenVertexArrays(1, &vao_);	//对vao赋值
	glBindVertexArray(vao_);

	//Create VBO
	glGenBuffers(1, &bufferName_);
	glBindBuffer(GL_ARRAY_BUFFER, bufferName_);
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
	// Make sure the VAO is not changed from outside code
	glBindVertexArray(0);
}

void VertexBuffer::InitQuad(
	float x1, float y1,
	float x2, float y2,
	float x3, float y3,
	float x4, float y4)
{
	//VBO data
	GLfloat vertexData[] =
	{
		x1, y1, 0.0f,1.0f,
		x2, y2, 1.0f,1.0f,
		x3, y3, 1.0f,0.0f,
		x4, y4, 0.0f,0.0f
	};

	//VAO
	glGenVertexArrays(1, &vao_);	//对vao赋值
	glBindVertexArray(vao_);

	//Create VBO
	glGenBuffers(1, &bufferName_);
	glBindBuffer(GL_ARRAY_BUFFER, bufferName_);
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
	// Make sure the VAO is not changed from outside code
	glBindVertexArray(0);
}