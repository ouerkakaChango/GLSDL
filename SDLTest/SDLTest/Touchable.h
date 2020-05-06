#pragma once
#include "Button.h"

#include "Utility.h"

class Image;

class Touchable :
	public Button
{
public:
	Touchable();

	void Render() override;

	Func beginOverlapFunc_;
	Func endOverlapFunc_;
protected:

	bool lastTouch_{ false };

};

