#pragma once

class Vector2D
{
public:
	static float dot(const Vector2D& a, const Vector2D& b) { return a._x * b._x + a._y * b._y; }
	static float cross(const Vector2D& a, const Vector2D& b) { return a._x * b._y - a._y * b._x; }

	Vector2D(float x, float y) : _x(x), _y(y) {}
	float len() { return sqrtf(_x * _x + _y * _y); }
	void normalize() { float length = len(); if (length == 0) return; _x /= length; _y /= length; }
	Vector2D operator*(float val) { return { _x * val, _y * val }; }
	Vector2D operator+(const Vector2D& obj) { return { _x + obj._x, _y + obj._y }; }

	float _x;
	float _y;
private:
};