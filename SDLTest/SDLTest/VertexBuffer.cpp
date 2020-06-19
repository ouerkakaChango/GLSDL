#include "VertexBuffer.h"



VertexBuffer::VertexBuffer()
{
}


VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::InitQuad(float scale)
{
	bQuad_ = true;
	dataQuad_ = new GLfloat[16];
	//VBO data
	//GLfloat vertexData[] =
	//{
	//	-scale, -scale, 0.0f,1.0f,
	//	 scale, -scale, 1.0f,1.0f,
	//	 scale,  scale, 1.0f,0.0f,
	//	-scale,  scale, 0.0f,0.0f
	//};
	dataQuad_[0] = -scale;	dataQuad_[1] = -scale;	dataQuad_[2] = 0.0f;	dataQuad_[3] = 1.0f;
	dataQuad_[4] = scale;	dataQuad_[5] = -scale;	dataQuad_[6] = 1.0f;	dataQuad_[7] = 1.0f;
	dataQuad_[8] = scale;	dataQuad_[9] = scale;	dataQuad_[10] = 1.0f;	dataQuad_[11] = 0.0f;
	dataQuad_[12] = -scale;	dataQuad_[13] = scale;	dataQuad_[14] = 0.0f;	dataQuad_[15] = 0.0f;

	//VAO
	glGenVertexArrays(1, &vao_);	//对vao赋值
	glBindVertexArray(vao_);

	//Create VBO
	glGenBuffers(1, &bufferName_);
	glBindBuffer(GL_ARRAY_BUFFER, bufferName_);
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(GLfloat), dataQuad_, GL_STATIC_DRAW);
	// Make sure the VAO is not changed from outside code
	glBindVertexArray(0);
}

void VertexBuffer::InitQuad(Rect quad, VBDrawType drawType)
{
	bQuad_ = true;
	dataQuad_ = new GLfloat[16];
	//VBO data
	//GLfloat vertexData[] =
	//{
	//	quad.x-quad.hw, -quad.y-quad.hh, 0.0f,1.0f,
	//	quad.x+quad.hw, -quad.y-quad.hh, 1.0f,1.0f,
	//	quad.x+quad.hw, -quad.y+quad.hh, 1.0f,0.0f,
	//	quad.x-quad.hw, -quad.y+quad.hh, 0.0f,0.0f
	//};
	dataQuad_[0] = quad.x - quad.hw;	dataQuad_[1] = -quad.y - quad.hh;	dataQuad_[2] = 0.0f;	dataQuad_[3] = 1.0f;
	dataQuad_[4] = quad.x + quad.hw;	dataQuad_[5] = -quad.y - quad.hh;	dataQuad_[6] = 1.0f;	dataQuad_[7] = 1.0f;
	dataQuad_[8] = quad.x + quad.hw;	dataQuad_[9] = -quad.y + quad.hh;	dataQuad_[10] = 1.0f;	dataQuad_[11] = 0.0f;
	dataQuad_[12] = quad.x - quad.hw;	dataQuad_[13] = -quad.y + quad.hh;	dataQuad_[14] = 0.0f;	dataQuad_[15] = 0.0f;

	//VAO
	glGenVertexArrays(1, &vao_);	//对vao赋值
	glBindVertexArray(vao_);

	//Create VBO
	glGenBuffers(1, &bufferName_);
	glBindBuffer(GL_ARRAY_BUFFER, bufferName_);
	if (drawType == VB_Static)
	{
		glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(GLfloat), dataQuad_, GL_STATIC_DRAW);
	}
	else if (drawType == VB_Dynamic)
	{
		glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(GLfloat), dataQuad_, GL_DYNAMIC_DRAW);
	}
	// Make sure the VAO is not changed from outside code
	glBindVertexArray(0);
}

void VertexBuffer::SetQuad(Rect quad)
{
	//VBO data
	//GLfloat vertexData[] =
	//{
	//	quad.x - quad.hw, -quad.y - quad.hh, 0.0f,1.0f,
	//	quad.x + quad.hw, -quad.y - quad.hh, 1.0f,1.0f,
	//	quad.x + quad.hw, -quad.y + quad.hh, 1.0f,0.0f,
	//	quad.x - quad.hw, -quad.y + quad.hh, 0.0f,0.0f
	//};

	dataQuad_[0] = quad.x - quad.hw;	dataQuad_[1] = -quad.y - quad.hh;
	dataQuad_[4] = quad.x + quad.hw;	dataQuad_[5] = -quad.y - quad.hh;
	dataQuad_[8] = quad.x + quad.hw;	dataQuad_[9] = -quad.y + quad.hh;
	dataQuad_[12] = quad.x - quad.hw;	dataQuad_[13] = -quad.y + quad.hh;

	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, bufferName_);

	glBufferSubData(GL_ARRAY_BUFFER,0, 4 * 4 * sizeof(GLfloat), dataQuad_);
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