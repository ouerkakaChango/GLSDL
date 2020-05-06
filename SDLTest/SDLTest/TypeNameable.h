#pragma once
#include <string>
class TypeNameable
{
public:
	TypeNameable();
	~TypeNameable();
	std::string typeName_;
	static std::string TypeName();
};

