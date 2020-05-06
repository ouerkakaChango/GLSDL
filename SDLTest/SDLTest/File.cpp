#include "File.h"
#include <fstream>
using namespace std;

File::File()
{
}


File::~File()
{
}

//https://stackoverflow.com/questions/29915545/reading-from-text-file-to-char-array
ReadFile::ReadFile(const Path& filePath)
{
	std::ifstream fin(filePath);
	// get pointer to associated buffer object
	std::filebuf* pbuf = fin.rdbuf();
	// get file size using buffer's members
	std::size_t size = static_cast<size_t>(pbuf->pubseekoff(0, fin.end, fin.in));
	pbuf->pubseekpos(0, fin.in);
	// allocate memory to contain file data
	buffer_ = new char[size+1];
	memset(buffer_, ' ', size);
	// get file data
	pbuf->sgetn(buffer_, size);
	buffer_[size] = '\0';
	fin.close();
}

const GLchar* ReadFile::GetGLCharBuffer()
{
	return static_cast<GLchar*>(buffer_);
}

std::vector<std::string> ReadFile::GetWords()
{
	if (!words_.empty()) { return words_; }
	char* p = buffer_;
	unsigned count = 0;
	while (*p != '\0')
	{
		if (*p == ' ' || *p == '\n' || *p == ';' || *p=='\t')
		{
			std::string temp(p-count, count);
			if (count != 0)
			{
				words_.push_back(temp);
			}
			count = 0;
		}
		else
		{
			count += 1;
		}
		p++;
	}
	return words_;
}