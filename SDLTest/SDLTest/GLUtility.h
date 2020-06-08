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

class RenderTexture;
//https://stackoverflow.com/questions/31254444/how-to-save-a-texture-as-an-image-file-using-libraries-related-to-opengl
void SaveRTToFile(const Path& path, RenderTexture* rt);

void checkgl();