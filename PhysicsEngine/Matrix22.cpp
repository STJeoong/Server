#include "pch.h"
#include "Matrix22.h"

#pragma region public
Matrix22::Matrix22(float radian)
{
	_radian = radian;
	float c = std::cosf(radian);
	float s = std::sinf(radian);
	_m00 = c; _m01 = -s;
	_m10 = s; _m11 = c;
}
void Matrix22::set(float radian)
{
	_radian = radian;
	float c = std::cosf(radian);
	float s = std::sinf(radian);
	_m00 = c; _m01 = -s;
	_m10 = s; _m11 = c;
}
Matrix22 Matrix22::transpose() const
{
	return Matrix22(-_radian);
}
Vector2D Matrix22::operator*(const Vector2D& obj) const
{
	float x = _m00 * obj.x() + _m01 * obj.y();
	float y = _m10 * obj.x() + _m11 * obj.y();
	return { x, y };
}
Matrix22 Matrix22::operator*(const Matrix22& obj) const
{
	return Matrix22(_radian + obj._radian);
}
Matrix22& Matrix22::operator*=(const Matrix22& obj)
{
	this->set(_radian + obj._radian);
	return *this;
}
#pragma endregion

#pragma region private
#pragma endregion