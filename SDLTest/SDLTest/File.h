#pragma once
#include "Utility.h"
#include "GLUtility.h"
class File
{
public:
	File();
	~File();
};

class ReadFile
{
public:
	ReadFile(const Path& filePath);
	const GLchar* GetGLCharBuffer();
	std::vector<std::string> GetWords();
protected:
	char* buffer_{ nullptr };
	std::vector<std::string> words_;
};
