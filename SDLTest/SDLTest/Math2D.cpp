#include "Math2D.h"

#include <cmath>
#include "God.h"

Math2D::Math2D()
{
}


Math2D::~Math2D()
{
}

Vec2::Vec2(float x, float y)
	:x_(x),
	y_(y)
{

}

Vec2 Vec2::operator+(const Vec2& v) const
{
	return Vec2(x_ + v.x_, y_ + v.y_);
}

Vec2 Vec2::operator-(const Vec2& v) const
{
	return Vec2(x_ - v.x_, y_ - v.y_);
}

Vec2 Vec2::operator-() const
{
	return Vec2(-x_, -y_);
}

Vec2 Vec2::operator/(float n) const
{
	return Vec2(x_ / n, y_ / n);
}

Vec2 Vec2::operator*(float n) const
{
	return Vec2(x_ * n, y_ * n);
}

Vec2 Vec2::GetNormal()
{
	return *this / Length();
}

float Vec2::Length()
{
	return sqrtf(x_*x_ + y_ * y_);
}

void Quad::VerticleLineQuad(Vec2 p1, Vec2 p2, Vec2 dir, Vec2 dir2, float width)
{
	v4_ = p1 - dir * width / 2;
	v3_ = v4_ + dir * width;

	v1_ = p2 - dir2 * width / 2;
	v2_ = v1_ + dir2 * width;
}

void Quad::HoriLineQuad(Vec2 p1, Vec2 p2, Vec2 dir, Vec2 dir2, float width)
{
	v4_ = p1 - dir * width / 2;
	v1_ = v4_ + dir * width;

	v3_ = p2 - dir2 * width / 2;
	v2_ = v3_ + dir2 * width;
}

Quad Quad::GetRenderQuad() const
{
	Quad re;
	re.v1_ = GetRenderPos(v1_);
	re.v2_ = GetRenderPos(v2_);
	re.v3_ = GetRenderPos(v3_);
	re.v4_ = GetRenderPos(v4_);
	return re;
}

Vec2 GetRenderPos(const Vec2& p)
{
	Vec2 re;
	re.x_ = p.x_ / GOD.windowW_ * 2.0f - 1.0f;
	re.y_ = -(p.y_ / GOD.windowH_ * 2.0f - 1.0f);
	return re;
}