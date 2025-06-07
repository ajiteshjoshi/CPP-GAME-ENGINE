#include "Vec2.h"
#include <math.h>

Vec2::Vec2()
{
}

Vec2::Vec2(float xin, float yin)
	: x(xin), y(yin)
{
}

bool Vec2::operator==(const Vec2& rhs) const
{
	return (x == rhs.x) && (y == rhs.y);
}

bool Vec2::operator!=(const Vec2& rhs) const
{
	return (x != rhs.x) || (y != rhs.y);
}

Vec2 Vec2::operator+(const Vec2& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator/(const float val) const
{
	return Vec2(x / val, y / val);
}

Vec2 Vec2::operator*(const float val) const
{
	return Vec2(x * val, y * val);
}

void Vec2::operator+=(const Vec2& rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
}

void Vec2::operator-=(const Vec2& rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
}

void Vec2::operator*=(const float val)
{
	this->x *= val;
	this->y *= val;
}

void Vec2::operator/=(const float val)
{
	this->x /= val;
	this->y /= val;
}

void Vec2::normalize()
{
	float len = this->length();
	this->x = this->x / len;
	this->y = this->y / len;
}

float Vec2::length() const
{
	return sqrtf((this->x * this->x) + (this->y * this->y));
}

float Vec2::cross(const Vec2& v) const
{
	return this->x * v.y - this->y * v.x;
}
