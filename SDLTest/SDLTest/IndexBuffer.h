#pragma once
#include "GLUtility.h"
class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	void InitQuad(GLuint vao);

	GLuint bufferName_{ 0 };
};

