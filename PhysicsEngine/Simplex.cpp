#include "pch.h"
#include "Simplex.h"
#include "Utils.h"
#include "Matrix22.h"

#pragma region public
void Simplex::init(const Point2D& pointFromA, const Point2D& pointFromB)
{
	//_points.resize(3);
	_points.clear();
	_size = 0;
	_containsOrigin = false;
	//_points[_size++] = pointFromA - pointFromB;
	_points.push_back(pointFromA - pointFromB);
	_supportVec = _points[0] * -1;
	if (_supportVec.x() == 0.0f && _supportVec.y() == 0.0f)
		_containsOrigin = true;
}
bool Simplex::insert(const Point2D& pointFromA, const Point2D& pointFromB)
{
	//if (_size == 3)
	if (_points.size() == 3)
	{
		_points[0] = _points[1];
		_points[1] = _points[2];
		_points[2] = pointFromA - pointFromB;
	}
	else
		_points.push_back(pointFromA - pointFromB);
		//_points[_size++] = pointFromA - pointFromB;
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
		// TODO : supportVec�� {0,0}�̸�? // ���� ������
		_supportVec = Vector2D::cross(Vector2D::cross(AB, _supportVec), AB);
		if (_supportVec.x() == 0.0f && _supportVec.y() == 0.0f)
			_containsOrigin = true;
		return true;
	}
	
	Point2D& A = _points[0];
	Point2D& B = _points[1];
	Vector2D CA = A - newPoint;
	Vector2D CB = B - newPoint;
	Vector2D CO = newPoint * -1;
	_supportVec = Vector2D::cross(Vector2D::cross(CA, CB), CB);
	if (Vector2D::dot(_supportVec, CO) > 0.0f) return true; // check Rbc. TODO : ���⿡ 0.0f ��ſ� FLT_EPSILON���� ������ �ణ ���������־ ok��
	_supportVec = Vector2D::cross(Vector2D::cross(CB, CA), CA);
	Utils::swap(_points[0], _points[1]);
	if (Vector2D::dot(_supportVec, CO) > 0.0f) return true; // check Rac
	_containsOrigin = true;
	return true;
}
#pragma endregion