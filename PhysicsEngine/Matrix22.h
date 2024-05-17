#pragma once
#include "Vector2D.h"
class Matrix22
{
public:
	Matrix22() { }
	Matrix22(float radian);
	void set(float radian);
	// in 2x2 회전행렬, 역행렬 = 전치행렬 = 반대로회전
	Matrix22 transpose() const;
	Vector2D operator*(const Vector2D& obj) const;
	Matrix22 operator*(const Matrix22& obj) const;
	Matrix22& operator*=(const Matrix22& obj);
	float radian() const { return _radian; }
private:
	Matrix22(float m00, float m01, float m10, float m11) : _m00(m00), _m01(m01), _m10(m10), _m11(m11) {}

	// 초기값은 radian == 0
	float _m00 = 1.0f;
	float _m01 = 0.0f;
	float _m10 = 0.0f;
	float _m11 = 1.0f;
	float _radian = 0.0f;
};