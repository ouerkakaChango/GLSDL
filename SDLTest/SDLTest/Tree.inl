#pragma once

template <class T>
void Tree<T>::AddChild(Tree<T>* child)
{
	child->father_ = static_cast<T*>(this);
	children_.push_back(static_cast<T*>(child));
}

template <class T>
void Tree<T>::AddChild(Tree<T>* child, unsigned n)
{
	child->father_ = static_cast<T*>(this);
	for (unsigned i = 0; i < n; i++)
	{
		children_.push_back(static_cast<T*>(child));
	}
}