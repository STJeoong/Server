#include "pch.h"
#include "Contact2D.h"
#include "Collider2D.h"
#include "GameObject.h"
#include "Line.h"
#include "CircleCollider.h"
#include "Utils.h"

#pragma region public
#pragma endregion

#pragma region private
void Contact2D::update()
{
	if (_isCircleA || _isCircleB)
	{
		this->processCircle();
		return;
	}

	if (_isEdgeA)
	{
		_normal = _colliderA->gameObject()->transform().rotation() * _localNormal;
		_contactB = _colliderB->toWorld(_localEdgePointsB[0]);
		Point2D globalEdgePointsA[2];
		globalEdgePointsA[0] = _colliderA->toWorld(_localEdgePointsA[0]);
		globalEdgePointsA[1] = _colliderA->toWorld(_localEdgePointsA[1]);
		Line line(globalEdgePointsA[0], globalEdgePointsA[1]);
		_contactA = line.project(_contactB);
		_depth = Vector2D::dot(_contactA - _contactB, _normal);
		// TODO : impulse�� �ʱ�ȭ�ؾߵǳ�? ���� �״���ϵ� �׽�Ʈ�غ���.
		if (Utils::outOfEdge(_contactA, globalEdgePointsA) && !this->changeEdge(_contactA, globalEdgePointsA))
			_depth = -10.0f;
	}
	else
	{
		_normal = _colliderB->gameObject()->transform().rotation() * _localNormal;
		_contactA = _colliderA->toWorld(_localEdgePointsA[0]);
		Point2D globalEdgePointsB[2];
		globalEdgePointsB[0] = _colliderB->toWorld(_localEdgePointsB[0]);
		globalEdgePointsB[1] = _colliderB->toWorld(_localEdgePointsB[1]);
		Line line(globalEdgePointsB[0], globalEdgePointsB[1]);
		_contactB = line.project(_contactA);
		_depth = Vector2D::dot(_contactA - _contactB, _normal);
		if (Utils::outOfEdge(_contactB, globalEdgePointsB) && !this->changeEdge(_contactB, globalEdgePointsB))
			_depth = -10.0f;
	}
	_tangent = Vector2D::cross(_normal, 1.0f);

	_rA = _contactA - _colliderA->position();
	_rB = _contactB - _colliderB->position();
	if (_depth < 0.0f)
		_normalImpulse = _tangentImpulse = 0.0f;
}
void Contact2D::processCircle()
{
	if (_isEdgeA) // A�� polygon, B�� ��
	{
		_normal = _colliderA->gameObject()->transform().rotation() * _localNormal;
		CircleCollider* circle = reinterpret_cast<CircleCollider*>(_colliderB);
		_contactB = circle->position() - _normal * circle->radius();
		Point2D globalEdgePointsA[2];
		globalEdgePointsA[0] = _colliderA->toWorld(_localEdgePointsA[0]);
		globalEdgePointsA[1] = _colliderA->toWorld(_localEdgePointsA[1]);
		Line line(globalEdgePointsA[0], globalEdgePointsA[1]);
		_contactA = line.project(_contactB);
		_depth = Vector2D::dot(_contactA - _contactB, _normal);
		_normalImpulse = _tangentImpulse = 0.0f;
		if (Utils::outOfEdge(_contactA, globalEdgePointsA)) // TODO : ���������� �ٲٴ� �ɷ� �ұ�?
			_depth = -10.0f;
	}
	else if (_isEdgeB) // A�� ��, B�� polygon
	{
		_normal = _colliderB->gameObject()->transform().rotation() * _localNormal;
		CircleCollider* circle = reinterpret_cast<CircleCollider*>(_colliderA);
		_contactA = circle->position() + _normal * circle->radius();
		Point2D globalEdgePointsB[2];
		globalEdgePointsB[0] = _colliderB->toWorld(_localEdgePointsB[0]);
		globalEdgePointsB[1] = _colliderB->toWorld(_localEdgePointsB[1]);
		Line line(globalEdgePointsB[0], globalEdgePointsB[1]);
		_contactB = line.project(_contactA);
		_depth = Vector2D::dot(_contactA - _contactB, _normal);
		_normalImpulse = _tangentImpulse = 0.0f;
		if (Utils::outOfEdge(_contactB, globalEdgePointsB))
			_depth = -10.0f;
	}
	else // �Ѵ� ��, ���̶� polygon ������
	{
		if (_isCircleA && _isCircleB) // �Ѵ� ��
		{
			_normal = (_colliderB->position() - _colliderA->position()).normalized();
			CircleCollider* circleA = reinterpret_cast<CircleCollider*>(_colliderA);
			CircleCollider* circleB = reinterpret_cast<CircleCollider*>(_colliderB);
			_contactA = circleA->position() + _normal * circleA->radius();
			_contactB = circleB->position() - _normal * circleB->radius();
			_normalImpulse = _tangentImpulse = 0.0f;
		}
		else // �ϳ��� ��
		{
			if (_isCircleA)
				_normal = _colliderA->gameObject()->transform().rotation() * _localNormal;
			else
				_normal = _colliderB->gameObject()->transform().rotation() * _localNormal;
			_contactA = _colliderA->toWorld(_localEdgePointsA[0]);
			_contactB = _colliderB->toWorld(_localEdgePointsB[0]);
		}
		_depth = Vector2D::dot(_contactA - _contactB, _normal);
	}
	_tangent = Vector2D::cross(_normal, 1.0f);

	_rA = _contactA - _colliderA->position();
	_rB = _contactB - _colliderB->position();
}
bool Contact2D::changeEdge(const Point2D& p, Point2D globalPoints[2])
{
	// isEdgeA, isEdgeB, normal, localNormal, _depth, localEdgePoints, globalPoint �����ؾߵ�
	if (_isEdgeA)
	{
		int idx = this->getCloserPoint(p, globalPoints);
		bool ret = _colliderB->project(_normal, false, globalPoints[idx], _contactB, _localEdgePointsB);
		if (!ret)
			return false;
		_isEdgeA = false;
		if (_localEdgePointsB[0] == _localEdgePointsB[1])
			_isEdgeB = false;
		else
			_isEdgeB = true;
		_contactA = globalPoints[idx];
		_localEdgePointsA[0] = _localEdgePointsA[1] = _colliderA->toLocal(_contactA);
		Point2D globalEdgePointsB[2];
		globalEdgePointsB[0] = _colliderB->toWorld(_localEdgePointsB[0]);
		globalEdgePointsB[1] = _colliderB->toWorld(_localEdgePointsB[1]);
		Line line(globalEdgePointsB[0], globalEdgePointsB[1]);
		_contactB = line.project(_contactA);
		Vector2D AB = _localEdgePointsB[1] - _localEdgePointsB[0];
		Vector2D AO = -1.0f * _localEdgePointsB[0];
		_localNormal = Vector2D::cross(Vector2D::cross(AB, AO), AB).normalized();
		_normal = _colliderB->gameObject()->transform().rotation() * _localNormal;
	}
	else
	{
		int idx = this->getCloserPoint(p, globalPoints);
		bool ret = _colliderA->project(_normal, true, globalPoints[idx], _contactA, _localEdgePointsA);
		if (!ret)
			return false;
		_isEdgeB = false;
		if (_localEdgePointsA[0] == _localEdgePointsA[1])
			_isEdgeA = false;
		else
			_isEdgeA = true;
		_contactB = globalPoints[idx];
		_localEdgePointsB[0] = _localEdgePointsB[1] = _colliderB->toLocal(_contactB);
		Point2D globalEdgePointsA[2];
		globalEdgePointsA[0] = _colliderA->toWorld(_localEdgePointsA[0]);
		globalEdgePointsA[1] = _colliderA->toWorld(_localEdgePointsA[1]);
		Line line(globalEdgePointsA[0], globalEdgePointsA[1]);
		_contactA = line.project(_contactB);
		Vector2D AB = _localEdgePointsA[1] - _localEdgePointsA[0];
		Vector2D AO = -1.0f * _localEdgePointsA[0];
		_localNormal = Vector2D::cross(Vector2D::cross(AO, AB), AB).normalized();
		_normal = _colliderA->gameObject()->transform().rotation() * _localNormal;
	}
	_depth = Vector2D::dot(_contactA - _contactB, _normal);
	return true;
}
int Contact2D::getCloserPoint(const Point2D& p, Point2D globalPoints[2])
{
	float dist1 = (p - globalPoints[0]).squaredLen();
	float dist2 = (p - globalPoints[1]).squaredLen();
	if (dist1 < dist2)
		return 0;
	return 1;
}
bool Contact2D::operator==(const Contact2D& other)
{
	if (_colliderA != other._colliderA || _colliderB != other._colliderB)
		return false;
	if (_isEdgeA && other._isEdgeA)
	{
		bool equalPoint = _localEdgePointsB[0] == other._localEdgePointsB[0];
		bool equalEdge = ((_localEdgePointsA[0] == other._localEdgePointsA[0] && _localEdgePointsA[1] == other._localEdgePointsA[1]) ||
			(_localEdgePointsA[1] == other._localEdgePointsA[0] && _localEdgePointsA[0] == other._localEdgePointsA[1]));
		bool equal = equalPoint && equalEdge;
		if (equal)
			return true;
	}
	else if (_isEdgeB && other._isEdgeB)
	{
		bool equalPoint = _localEdgePointsA[0] == other._localEdgePointsA[0];
		bool equalEdge = ((_localEdgePointsB[0] == other._localEdgePointsB[0] && _localEdgePointsB[1] == other._localEdgePointsB[1]) ||
			(_localEdgePointsB[1] == other._localEdgePointsB[0] && _localEdgePointsB[0] == other._localEdgePointsB[1]));
		bool equal = equalPoint && equalEdge;
		if (equal)
			return true;
	}
	return false;
}
#pragma endregion