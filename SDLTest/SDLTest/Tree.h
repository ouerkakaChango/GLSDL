#pragma once
#include "Utility.h"

template <class T>
class Tree
{
public:
	void AddChild(Tree<T>* child);
	void AddChild(Tree<T>* child,unsigned n);
protected:
	T* father_;
	std::vector<T*> children_;
};

#include "Tree.inl"