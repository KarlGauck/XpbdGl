#pragma once

struct Vec2 
{
	Vec2();
	Vec2(float x, float y);
	Vec2(int x, int y);

	float x, y;

	void normalize();
	Vec2 normalized();

	float dot(const Vec2& other);

	float distance(const Vec2& other);

	Vec2 operator+(const Vec2& other);
	void operator+=(const Vec2& other);

	Vec2 operator-(const Vec2& other);
	void operator-=(const Vec2& other);

	Vec2 operator*(float scalar);
	void operator*=(float scalar);

	Vec2 operator/(float divisor);
	void operator/=(float divisor);
};