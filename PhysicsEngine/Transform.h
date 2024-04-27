#pragma once
#include "Vector2D.h"
#include "Matrix22.h"
class Transform
{
public:
	Transform() {}
	Transform(const Point2D& p, float radian) : _pos(p), _rot(radian) {}
	void set(const Point2D& p, float radian) { _pos = p; _rot.set(radian); }
	const Point2D& position() const { return _pos; }
	const Matrix22& rotation() const { return _rot; }
	void move(const Vector2D& v) { _pos += v; }
	void rotate(float radian) { Matrix22 mat{ radian }; _rot *= mat; }
private:

	Point2D _pos;
	Matrix22 _rot;
};