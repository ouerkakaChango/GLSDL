#include "GLUtility.h"

#include <stdio.h>

#include "God.h"
#include "RenderTexture.h"

GLUtility::GLUtility()
{
}


GLUtility::~GLUtility()
{
}

void printShaderLog(GLuint shader)
{
	//Make sure name is shader
	if (glIsShader(shader))
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a shader\n", shader);
	}
}

void printProgramLog(GLuint program)
{
	//Make sure name is shader
	if (glIsProgram(program))
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
	}
}

void SaveRTToFile(const Path& path, RenderTexture* rt)
{
	FILE    *output_image;
	int     output_width, output_height;

	//??? to think,should be rt's w/h,but that can't run normally
	output_width = GOD.windowW_;
	output_height = GOD.windowH_;

	/// READ THE PIXELS VALUES from FBO AND SAVE TO A .PPM FILE
	int             i, j, k;
	unsigned char   *pixels = (unsigned char*)malloc(output_width*output_height * 3);

	/// READ THE CONTENT FROM THE FBO
	glBindFramebuffer(GL_READ_FRAMEBUFFER, rt->frameBufferID_);
	glReadPixels(0, 0, output_width, output_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	output_image = fopen(path.c_str(), "wt");
	fprintf(output_image, "P3\n");
	fprintf(output_image, "# Created by Ricao\n");
	fprintf(output_image, "%d %d\n", output_width, output_height);
	fprintf(output_image, "255\n");

	k = 0;
	for (i = 0; i < output_width; i++)
	{
		for (j = 0; j < output_height; j++)
		{
			fprintf(output_image, "%u %u %u ", (unsigned int)pixels[k], (unsigned int)pixels[k + 1],
				(unsigned int)pixels[k + 2]);
			k = k + 3;
		}
		fprintf(output_image, "\n");
	}
	free(pixels);

}

void checkgl()
{
	GLenum t = glGetError();
	if (t != GL_NO_ERROR)
	{
		abort();
	}
}