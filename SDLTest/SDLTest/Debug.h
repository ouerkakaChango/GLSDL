#pragma once
#include <iostream>
#include <string>
#include <stdio.h>

class Debug
{
public:
	Debug();
	~Debug();
};

template <class T>
void Log(std::string varName,T t){std::cout << "### " << varName << " : "<<t<<std::endl; }

template <class T1,class T2>
void Log(std::string name1, T1 t1, std::string name2, T2 t2) { std::cout << "### " << name1 <<","<< name2 << " : " << t1<<" "<<t2 << std::endl; }

#define LOG(a) Log(#a,a)
#define LOG2(a1,a2) Log(#a1,a1,#a2,a2)