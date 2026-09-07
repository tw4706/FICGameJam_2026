#include "Vector2.h"
#include<Dxlib.h>
#include<cmath>

Vector2::Vector2(float x, float y):
	x(x),
	y(y)
{
}

float Vector2::Length() const
{
	return std::hypot(x, y);
}

void Vector2::Normalize()
{
	auto len = Length();
	if (len == 0.0f)
	{
		x = 0.0f;
		y = 0.0f;
		return;
	}
	x /= len;
	y /= len;
}

Vector2 Vector2::Normalized() const
{
	auto len = Length();
	if (len == 0.0f)
	{
		return{ 0.0f,0.0f };
	}
	return { x / len,y / len };
}

Vector2 Vector2::operator-()const
{
	return{ -x,-y };
}

void Vector2::operator+=(const Vector2& val)
{
	x += val.x;
	y += val.y;
}

void Vector2::operator-=(const Vector2& val)
{
	x -= val.x;
	y -= val.y;
}

void Vector2::operator*=(const float scale)
{
	x *= scale;
	y *= scale;
}

Vector2 Vector2::operator+(const Vector2& val) const
{
	return { x + val.x,y + val.y };
}

Vector2 Vector2::operator-(const Vector2& val) const
{
	return { x - val.x,y - val.y };
}

Vector2 Vector2::operator*(float scale) const
{
	return { x * scale,y * scale };
}

float Vector2::Lerp(float a, float b, float t) const
{
	return a + (b - a) * t;
}
