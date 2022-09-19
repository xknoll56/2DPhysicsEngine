#pragma once

#include <math.h>
struct vec2
{
	float x, y;

	vec2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	float mag()
	{
		return sqrtf(x * x + y * y);
	}

	void normalize()
	{
		float mag = this->mag();
		x /= mag;
		y /= mag;
	}

	vec2& operator+=(const vec2& a)
	{
		this->x += a.x;
		this->y += a.y;
	}
};

vec2 operator+(vec2 a, vec2 b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

vec2 operator-(vec2 a, vec2 b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

vec2 operator*(vec2 a, float f)
{
	a.x *= f;
	a.y *= f;
	return a;
}

vec2 operator*(float f, vec2 a)
{
	a.x *= f;
	a.y *= f;
	return a;
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
