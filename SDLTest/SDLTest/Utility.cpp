#include "Utility.h"

Utility::Utility()
{
}


Utility::~Utility()
{
}

void sure(bool b)
{
	if (!b)
	{
		abort();
	}
}

bool IsExtention(const Path& filePath, const std::string& extention)
{
	return filePath.find(extention) == filePath.size() - extention.size();
}

bool InRect(int x, int y, SDL_Rect rect)
{
	int dx = abs(x - rect.x);
	int dy = abs(y - rect.y);
	return dx <= rect.w && dy <= rect.h;
}

float Lerp(float min, float max, float k)
{
	if (k <= 0)
	{
		return min;
	}
	else if (k >= 1)
	{
		return max;
	}
	else
	{
		return min * (1 - k) + max * k;
	}
}

float FlashFunc(float k)
{
	if (k >= 0 && k <= 0.5f)
	{
		return k / 0.5f;
	}
	else
	{
		return (1 - k) / 0.5f;
	}
}