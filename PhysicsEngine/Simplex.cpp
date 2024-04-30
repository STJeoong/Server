#include "pch.h"
#include "Simplex.h"

#pragma region public
void Simplex::init(const Point2D& p)
{
	_state = 0;
	_points.resize(3);
	_points[0] = p;
	_supportVec = _points[0] * -1;
}
bool Simplex::insert(const Point2D& p)
{
	if (_state < 2)
	{
		++_state;
		_points[_state] = p;
	}
	else if (_state == 2)
	{
		_points[0] = _points[1];
		_points[1] = _points[2];
		_points[2] = p;
	}

	return this->check();
}
#pragma endregion

#pragma region private
bool Simplex::check()
{
	// 새로운 점이 원점 건너편에 있는지 체크 아니면 무조건 충돌가능성 없음.
	float result = Vector2D::dot(_supportVec, _points[_state]);
	if (result < 0.0f) return false;

	// compute new support vector
	if (_state == 1)
	{
		Vector2D AB = _points[1] - _points[0];
		_supportVec = Vector2D::cross(Vector2D::cross(AB, _supportVec), AB);
		return true;
	}
	
	Vector2D CA = _points[0] - _points[2];
	Vector2D CB = _points[1] - _points[2];
	Vector2D CO = _points[2] * -1;
	_supportVec = Vector2D::cross(Vector2D::cross(CA, CB), CB);
	if (Vector2D::dot(_supportVec, CO) > 0.0f) return true; // check Rbc. TODO : 여기에 0.0f 대신에 FLT_EPSILON쓰면 원점이 약간 삐져나가있어도 ok함
	_supportVec = Vector2D::cross(Vector2D::cross(CB, CA), CA);
	if (Vector2D::dot(_supportVec, CO) > 0.0f) return true; // check Rac
	_containsOrigin = true;
	return true;
}
#pragma endregion