#pragma once
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

class GLUtility
{
public:
	GLUtility();
	~GLUtility();
};

void printShaderLog(GLuint shader);
void printProgramLog(GLuint program);