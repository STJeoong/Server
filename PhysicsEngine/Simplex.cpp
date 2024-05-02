#include "pch.h"
#include "Simplex.h"

#pragma region public
void Simplex::init(const Point2D& p)
{
	_points.clear();
	_points.push_back(p);
	_supportVec = _points[0] * -1;
}
bool Simplex::insert(const Point2D& p)
{
	_points.push_back(p);
	return this->check();
}
#pragma endregion

#pragma region private
bool Simplex::check()
{
	// ���ο� ���� ���� �ǳ��� �ִ��� üũ �ƴϸ� ������ �浹���ɼ� ����.
	const Point2D& newPoint = _points[_points.size() - 1];
	float result = Vector2D::dot(_supportVec, newPoint);
	if (result < 0.0f) return false;

	// if 1-simplex
	if (_points.size() == 2)
	{
		Vector2D AB = _points[1] - _points[0];
		_supportVec = Vector2D::cross(Vector2D::cross(AB, _supportVec), AB);
		return true;
	}
	
	const Point2D& A = _points[_points.size() - 3];
	const Point2D& B = _points[_points.size() - 2];
	Vector2D CA = A - newPoint;
	Vector2D CB = B - newPoint;
	Vector2D CO = newPoint * -1;
	_supportVec = Vector2D::cross(Vector2D::cross(CA, CB), CB);
	if (Vector2D::dot(_supportVec, CO) > 0.0f) return true; // check Rbc. TODO : ���⿡ 0.0f ��ſ� FLT_EPSILON���� ������ �ణ ���������־ ok��
	_supportVec = Vector2D::cross(Vector2D::cross(CB, CA), CA);
	if (Vector2D::dot(_supportVec, CO) > 0.0f) return true; // check Rac
	_containsOrigin = true;
	return true;
}
#pragma endregion