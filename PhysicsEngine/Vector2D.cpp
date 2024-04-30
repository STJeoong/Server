#include "pch.h"
#include "Vector2D.h"
#include "Matrix22.h"

#pragma region public
float Vector2D::dot(const Vector2D& a, const Vector2D& b) { return a._x * b._x + a._y * b._y; }
float Vector2D::cross(const Vector2D& a, const Vector2D& b) { return a._x * b._y - a._y * b._x; }
Vector2D Vector2D::cross(const Vector2D& v, float z) { return { v._y * z, v._x * z * -1 }; }
Vector2D Vector2D::cross(float z, const Vector2D& v) { return { v._y * z * -1, v._x * z }; }

Vector2D::Vector2D(float x, float y) { _x = x; _y = y; }
float Vector2D::len() const { return sqrtf(_x * _x + _y * _y); }
float Vector2D::squaredLen() const { return _x * _x + _y * _y; }
void Vector2D::normalize() { float length = len(); if (length == 0) return; _x /= length; _y /= length; }
Vector2D Vector2D::normalized() const
{
	Vector2D ret = *this;
	ret.normalize();
	return ret;
}
void Vector2D::rotate(const Vector2D& origin, float radian)
{
	Matrix22 rotMatrix(radian);
	Vector2D vec(*this - origin);

	*this = origin + rotMatrix * vec;
}
void Vector2D::rotate(const Vector2D& origin, const Matrix22& mat)
{
	Vector2D vec(*this - origin);

	*this = origin + mat * vec;
}
Vector2D Vector2D::operator*(float val) const { return { _x * val, _y * val }; }
Vector2D& Vector2D::operator*=(float val) { _x *= val; _y *= val; return *this; }
Vector2D Vector2D::operator+(const Vector2D& obj) const { return { _x + obj._x, _y + obj._y }; }
Vector2D Vector2D::operator-(const Vector2D& obj) const { return { _x - obj._x, _y - obj._y }; }
Vector2D& Vector2D::operator+=(const Vector2D& obj) { _x += obj._x; _y += obj._y; return *this; }
#pragma endregion

#pragma region private
#pragma endregion