#pragma once

#include <math.h>
struct Vec2
{
	float x, y;

	Vec2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vec2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	float mag()
	{
		return sqrtf(x * x + y * y);
	}

	void normalize()
	{
		float mag = this->mag();
		if (mag > 0)
		{
			x /= mag;
			y /= mag;
		}
		else
		{
			x = 1.0f;
			y = 0.0f;
		}
	}

	Vec2 operator+=(const Vec2& a)
	{
		this->x += a.x;
		this->y += a.y;
		return *this;
	}

	Vec2 operator-=(const Vec2& a)
	{
		this->x -= a.x;
		this->y -= a.y;
		return *this;
	}

	bool operator==(const Vec2& other)
	{
		return other.x == x && other.y == y;
	}

	Vec2 operator-()
	{
		Vec2 ret;
		ret.x = -this->x;
		ret.y = -this->y;
		return ret;
	}
};

Vec2 operator+(Vec2 a, Vec2 b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

Vec2 operator-(Vec2 a, Vec2 b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

Vec2 operator*(Vec2 a, float f)
{
	a.x *= f;
	a.y *= f;
	return a;
}

Vec2 operator*(float f, Vec2 a)
{
	a.x *= f;
	a.y *= f;
	return a;
}

float Dot(Vec2 a, Vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

float Cross(Vec2 a, Vec2 b)
{
	return a.x * b.y - a.y * b.x;
}

Vec2 Tangent(Vec2 a)
{
	return Vec2{ -a.y, a.x };
}

struct RGBA
{
	float r, g, b, a;

	RGBA(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};
