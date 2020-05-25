#pragma once
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include "Utility.h"



class GLUtility
{
public:
	GLUtility();
	~GLUtility();
};

void printShaderLog(GLuint shader);
void printProgramLog(GLuint program);

//https://stackoverflow.com/questions/31254444/how-to-save-a-texture-as-an-image-file-using-libraries-related-to-opengl
void SaveRTToFile(const Path& path);
void SaveRTToFile4(const Path& path);

void checkgl();