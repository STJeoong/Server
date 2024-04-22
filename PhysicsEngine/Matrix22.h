#pragma once
#include "Vector2D.h"
class Matrix22
{
public:
	Matrix22(float radian) { float c = cosf(radian); float s = sinf(radian); _m00 = c; _m01 = -s; _m10 = s; _m11 = c; }
	// in 2x2, 역행렬 = 전치행렬
	Matrix22 transpose() { return { _m00, _m10, _m01, _m11 }; }
	Matrix22 absolute() { return { abs(_m00), abs(_m01), abs(_m10), abs(_m11) }; }
	Vector2D operator*(const Vector2D& obj) const { return { _m00 * obj.x() + _m01 * obj.y(), _m10 * obj.x() + _m11 * obj.y() }; }
	Matrix22 operator*(float val) const { return { _m00 * val, _m01 * val, _m10 * val, _m11 * val }; }
	Matrix22 operator*(const Matrix22& obj) const { return { _m00 * obj._m00 + _m01 * obj._m10, _m00 * obj._m01 + _m01 * obj._m11,
														_m10 * obj._m00 + _m11 * obj._m10, _m10 * obj._m01 + _m11 * obj._m11}; }
private:
	Matrix22(float m00, float m01, float m10, float m11) : _m00(m00), _m01(m01), _m10(m10), _m11(m11) {}

	float _m00;
	float _m01;
	float _m10;
	float _m11;
};