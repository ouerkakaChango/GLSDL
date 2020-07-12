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

bool ParamPack::operator==(const ParamPack& p) const
{
	return objPtr_ == p.objPtr_ &&
		vec2_ == p.vec2_;
}

bool ParamPack::operator!=(const ParamPack& p) const
{
	return !((*this) == p);
}

void InsertPackParam(ParamPack& re, Object* ptr)
{
	re.objPtr_ = ptr;
}

void InsertPackParam(ParamPack& re, Vec2 v2)
{
	re.vec2_ = v2;
}