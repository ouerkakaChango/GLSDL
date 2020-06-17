#pragma once

#include "Utility.h"


template <class T>
class Tree
{
public:
	void AddChild(T* child);
	void AddChild(T* child,unsigned n);
	T* father_{nullptr};
	std::vector<T*> children_;
};

//use smart ptr,when del father,children auto del
template <class T>
class AutoTree
{
public:
	void AddChild(shared_ptr<T> child);
	T* father_{nullptr};
	std::vector<shared_ptr<T>> children_;
};

#include "Tree.inl"