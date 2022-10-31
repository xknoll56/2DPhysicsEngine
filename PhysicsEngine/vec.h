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

	vec2()
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

	vec2 operator+=(const vec2& a)
	{
		this->x += a.x;
		this->y += a.y;
		return *this;
	}

	vec2 operator-=(const vec2& a)
	{
		this->x -= a.x;
		this->y -= a.y;
		return *this;
	}

	bool operator==(const vec2& other)
	{
		return other.x == x && other.y == y;
	}

	vec2 operator-()
	{
		vec2 ret;
		ret.x = -this->x;
		ret.y = -this->y;
		return ret;
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

float Dot(vec2 a, vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

float Cross(vec2 a, vec2 b)
{
	return a.x * b.y - a.y * b.x;
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
