#include "pch.h"
#include "Simplex.h"
#include "Utils.h"
#include "Matrix22.h"

#pragma region public
void Simplex::init(const Point2D& pointFromA, const Point2D& pointFromB)
{
	_points.clear();
	_sources.clear();
	_containsOrigin = false;
	_points.push_back(pointFromA - pointFromB);
	_sources.push_back({ pointFromA, pointFromB });
	_supportVec = _points[0] * -1;
	if (_supportVec == Vector2D(0.0f, 0.0f))
		_containsOrigin = true;
}
bool Simplex::insert(const Point2D& pointFromA, const Point2D& pointFromB)
{
	if (_points.size() == 3)
	{
		_points[0] = _points[1];
		_points[1] = _points[2];
		_points[2] = pointFromA - pointFromB;

		_sources[0] = _sources[1];
		_sources[1] = _sources[2];
		_sources[2] = { pointFromA, pointFromB };
	}
	else
	{
		_points.push_back(pointFromA - pointFromB);
		_sources.push_back({ pointFromA, pointFromB });
	}
	return this->check();
}
#pragma endregion

#pragma region private
bool Simplex::check()
{
	// 새로운 점이 원점 건너편에 있는지 체크 아니면 무조건 충돌가능성 없음.
	const Point2D& newPoint = _points[_points.size() - 1];
	float result = Vector2D::dot(_supportVec, newPoint);
	if (result < 0.0f) return false;

	// if 1-simplex
	if (_points.size() == 2)
	{
		Vector2D AB = _points[1] - _points[0];
		// TODO : supportVec가 {0,0}이면? // 원점 포함임
		_supportVec = Vector2D::cross(Vector2D::cross(AB, _supportVec), AB);
		if (_supportVec == Vector2D(0.0f, 0.0f))
			_containsOrigin = true;
		return true;
	}
	
	Point2D& A = _points[0];
	Point2D& B = _points[1];
	Vector2D CA = A - newPoint;
	Vector2D CB = B - newPoint;
	Vector2D CO = newPoint * -1;
	_supportVec = Vector2D::cross(Vector2D::cross(CA, CB), CB);
	if (Vector2D::dot(_supportVec, CO) > 0.0f) return true; // check Rbc. TODO : 여기에 0.0f 대신에 FLT_EPSILON쓰면 원점이 약간 삐져나가있어도 ok함
	_supportVec = Vector2D::cross(Vector2D::cross(CB, CA), CA);
	Utils::swap(_points[0], _points[1]);
	Utils::swap(_sources[0], _sources[1]);
	if (Vector2D::dot(_supportVec, CO) > 0.0f) return true; // check Rac
	_containsOrigin = true;
	return true;
}
#pragma endregion