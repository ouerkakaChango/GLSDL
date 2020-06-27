#pragma once

class Math2D
{
public:
	Math2D();
	~Math2D();
};

class Vec2
{
public:
	Vec2(float x = 0, float y = 0);
	Vec2 operator+(const Vec2& v) const;
	Vec2 operator-(const Vec2& v) const;
	Vec2 operator-() const;
	Vec2 operator/(float n) const;
	Vec2 operator*(float n) const;
	Vec2 GetNormal();
	float Length();
	float x_, y_;
};

Vec2 GetRenderPos(const Vec2& p);

class Quad
{
public:
	Quad() = default;
	void VerticleLineQuad(Vec2 p1, Vec2 p2, Vec2 dir, Vec2 dir2, float width);
	void HoriLineQuad(Vec2 p1, Vec2 p2, Vec2 dir, Vec2 dir2, float width);
	Quad GetRenderQuad() const;

	Vec2 v1_, v2_, v3_, v4_;
};