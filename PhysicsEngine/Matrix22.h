#pragma once
#include "Vector2D.h"
class Matrix22
{
public:
	Matrix22() { }
	Matrix22(float radian) { _radian = radian; float c = cosf(radian); float s = sinf(radian); _m00 = c; _m01 = -s; _m10 = s; _m11 = c; }
	void set(float radian) { _radian = radian; float c = cosf(radian); float s = sinf(radian); _m00 = c; _m01 = -s; _m10 = s; _m11 = c; }
	// in 2x2 회전행렬, 역행렬 = 전치행렬 = 반대로회전
	Matrix22 transpose() { return { _m00, _m10, _m01, _m11 }; }
	Matrix22 absolute() { return { abs(_m00), abs(_m01), abs(_m10), abs(_m11) }; }
	Vector2D operator*(const Vector2D& obj) const { return { _m00 * obj.x() + _m01 * obj.y(), _m10 * obj.x() + _m11 * obj.y() }; }
	Matrix22 operator*(const Matrix22& obj) const
	{
		Matrix22 ret;
		ret._m00 = _m00 * obj._m00 + _m01 * obj._m10;
		ret._m01 = _m00 * obj._m01 + _m01 * obj._m11;
		ret._m10 = _m10 * obj._m00 + _m11 * obj._m10;
		ret._m11 = _m10 * obj._m01 + _m11 * obj._m11;
		ret._radian = _radian + obj._radian;
		return ret;
	}
	void operator*=(const Matrix22& obj) { (*this) = (*this) * obj; _radian += obj._radian; }
	const float& radian() const { return _radian; }
private:
	Matrix22(float m00, float m01, float m10, float m11) : _m00(m00), _m01(m01), _m10(m10), _m11(m11) {}

	// 초기값은 radian == 0
	float _m00 = 1.0f;
	float _m01 = 0.0f;
	float _m10 = 0.0f;
	float _m11 = 1.0f;
	float _radian = 0.0f;
};