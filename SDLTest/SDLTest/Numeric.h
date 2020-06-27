#pragma once
#include <cmath>
#include "Math2D.h"

class Numeric
{
public:
	Numeric();
	~Numeric();
};

bool Equals(float a, float b);

bool Equals(float a, float b, float errorAccept);

struct Rect
{
	float x, y;
	float hw, hh; //half w,h
};

#define PI 3.1415926f