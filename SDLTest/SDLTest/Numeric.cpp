#include "Numeric.h"



Numeric::Numeric()
{
}


Numeric::~Numeric()
{
}

bool Equals(float a, float b)
{
	return abs(a - b) < 0.000001f;
}

bool Equals(float a, float b, float errorAccept)
{
	return abs(a - b) < errorAccept;
}