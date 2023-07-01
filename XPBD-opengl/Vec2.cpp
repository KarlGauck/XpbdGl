#include <math.h>

#include "Vec2.h"

Vec2::Vec2() 
{
	x = 0.0f;
	y = 0.0f;
}

Vec2::Vec2(float x, float y) : x(x), y(y) {}

Vec2::Vec2(int x, int y) : x((float)x), y((float)y) {}

void Vec2::operator+=(const Vec2& other)
{
	x += other.x;
	y += other.y;
}

void Vec2::operator-=(const Vec2& other)
{
	x -= other.x;
	y -= other.y;
}

void Vec2::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
}

void Vec2::operator/=(float divisor)
{
	x /= divisor;
	y /= divisor;
}

Vec2 Vec2::operator+(const Vec2& other) 
{
	return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other)
{
	return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(float scalar)
{
	return Vec2(x * scalar, y * scalar);
}

Vec2 Vec2::operator/(float divisor)
{
	return Vec2(x / divisor, y / divisor);
}

float Vec2::dot(const Vec2& other)
{
	return x * other.x + y * other.y;
}

float Vec2::distance(const Vec2& other)
{
	return (float) sqrt(pow(x-other.x, 2) + pow(y-other.y, 2));
}

void Vec2::normalize()
{
	float length = (float)sqrt(pow(x, 2) + pow(y, 2));
	if (length != 0.f) 
	{
		x /= length;
		y /= length;
	}
}


Vec2 Vec2::normalized()
{
	float length = (float)sqrt(pow(x, 2) + pow(y, 2));
	if (length != 0.f)
		return Vec2(x / length, y / length);
	return Vec2(x, y);
}
