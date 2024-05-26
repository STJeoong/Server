#include "pch.h"
#include "ContactGenerator.h"
#include "Collision2D.h"
#include "Collider2D.h"
#include "Contact2D.h"
#include "Vector2D.h"
#include "Matrix22.h"
#include "Line.h"
#include "ObjectPool.h"
#include "GameObject.h"

#pragma region public
ContactGenerator::ContactGenerator(Collision2D& collision, const Vector2D& normal) : _normal(normal)
{
	Collider2D* colliderA = collision.colliderA();
	Collider2D* colliderB = collision.colliderB();

	if (colliderA->perpendicularToTheSide(normal))
	{
		_A[0] = colliderA->computeSupportPoint(Matrix22(0.001f) * normal);
		_A[1] = colliderA->computeSupportPoint(Matrix22(-0.001f) * normal);
	}
	else
		_A[0] = _A[1] = colliderA->computeSupportPoint(normal);
	if (colliderB->perpendicularToTheSide(normal))
	{
		_B[0] = colliderB->computeSupportPoint(Matrix22(0.001f) * (normal * -1.0f));
		_B[1] = colliderB->computeSupportPoint(Matrix22(-0.001f) * (normal * -1.0f));
	}
	else
		_B[0] = _B[1] = colliderB->computeSupportPoint(normal * -1.0f);
	// check if it contacts to collider's edge
	_isVertexA = _A[0] == _A[1];
	if (colliderA->isCircle())
		_isVertexA = true;
	_isVertexB = _B[0] == _B[1];
	if (colliderB->isCircle())
		_isVertexB = true;

	if (!_isVertexA && !_isVertexB) // 면 대 면 접촉. contact 2개 생성.
	{
		this->preProcess();
		_contactNum = 2;
	}
	else if (_isVertexA && _isVertexB) // (사각형 꼭짓점, 원), (원, 원) 충돌일 때 가능. contact 1개 생성
		_contactNum = 1;
	else // 면 대 점 접촉. contact 1개 생성
		_contactNum = 1;
}
Contact2D* ContactGenerator::generate(Collision2D& collision, int idx)
{
	if (!_isVertexA && !_isVertexB) // 면 대 면 접촉. contact 2개 생성.
		return this->EdgeToEdge(collision, idx);
	else if (_isVertexA && _isVertexB) // (사각형 꼭짓점, 원), (원, 원) 충돌일 때 가능. contact 1개 생성
		return this->PointToPoint(collision);
	else // 면 대 점 접촉. contact 1개 생성
		return this->EdgeToPoint(collision);
}
#pragma endregion

#pragma region private
Contact2D* ContactGenerator::EdgeToEdge(Collision2D& collision, int idx)
{
	bool isEdgeA;
	bool isEdgeB;
	if (_isParallelToY)
	{
		if (_A[idx].y() > _B[idx].y())
		{
			isEdgeA = idx == 0 ? false : true;
			isEdgeB = idx == 0 ? true : false;
		}
		else
		{
			isEdgeA = idx == 0 ? true : false;
			isEdgeB = idx == 0 ? false : true;
		}
	}
	else
	{
		if (_A[idx].x() > _B[idx].x())
		{
			isEdgeA = idx == 0 ? false : true;
			isEdgeB = idx == 0 ? true : false;
		}
		else
		{
			isEdgeA = idx == 0 ? true : false;
			isEdgeB = idx == 0 ? false : true;
		}
	}
	return this->generateContact(collision, isEdgeA, isEdgeB, idx);
}
Contact2D* ContactGenerator::EdgeToPoint(Collision2D& collision)
{
	if (_isVertexA) // A는 점 접촉, B는 면 접촉
		return this->generateContact(collision, false, true, 0);
	else // A는 면 접촉, B는 점 접촉
		return this->generateContact(collision, true, false, 0);
}
Contact2D* ContactGenerator::PointToPoint(Collision2D& collision)
{
	return this->generateContact(collision, false, false, 0);
}
Contact2D* ContactGenerator::generateContact(Collision2D& collsion, bool isEdgeA, bool isEdgeB, int vertexIdx)
{
	Collider2D* colliderA = collsion.colliderA();
	Collider2D* colliderB = collsion.colliderB();
	Contact2D* c = ObjectPool::get<Contact2D>();

	c->_isEdgeA = isEdgeA;
	c->_isEdgeB = isEdgeB;
	c->_colliderA = colliderA;
	c->_colliderB = colliderB;
	c->_normal = _normal;
	c->_tangent = Vector2D::cross(_normal, 1.0f);
	c->_isCircleA = colliderA->isCircle();
	c->_isCircleB = colliderB->isCircle();
	if (isEdgeA)
	{
		Line line(_A[0], _A[1]);
		c->_contactA = line.project(_B[vertexIdx]);
		c->_contactB = _B[vertexIdx];
		c->_localEdgePointsA[0] = colliderA->toLocal(_A[0]);
		c->_localEdgePointsA[1] = colliderA->toLocal(_A[1]);
		c->_localEdgePointsB[0] = c->_localEdgePointsB[1] = colliderB->toLocal(_B[vertexIdx]);
		c->_localNormal = colliderA->gameObject()->transform().rotation().transpose() * _normal;
	}
	else if (isEdgeB)
	{
		Line line(_B[0], _B[1]);
		c->_contactA = _A[vertexIdx];
		c->_contactB = line.project(_A[vertexIdx]);
		c->_localEdgePointsA[0] = c->_localEdgePointsA[1] = colliderA->toLocal(_A[vertexIdx]);
		c->_localEdgePointsB[0] = colliderB->toLocal(_B[0]);
		c->_localEdgePointsB[1] = colliderB->toLocal(_B[1]);
		c->_localNormal = colliderB->gameObject()->transform().rotation().transpose() * _normal;
	}
	else
	{
		c->_contactA = _A[0];
		c->_contactB = _B[0];
		c->_localEdgePointsA[0] = c->_localEdgePointsA[1] = colliderA->toLocal(_A[0]);
		c->_localEdgePointsB[0] = c->_localEdgePointsB[1] = colliderB->toLocal(_B[0]);
		if (c->_isCircleA)
			c->_localNormal = colliderA->gameObject()->transform().rotation().transpose() * _normal;
		else
			c->_localNormal = colliderB->gameObject()->transform().rotation().transpose() * _normal;
	}
	c->_depth = Vector2D::dot(c->_contactA - c->_contactB, _normal);
	c->_rA = c->_contactA - colliderA->position();
	c->_rB = c->_contactB - colliderB->position();
	return c;
}
void ContactGenerator::preProcess()
{
	static const float TOLERANCE = 0.0001f;
	if (std::abs(_A[0].x() - _A[1].x()) < TOLERANCE)
		_isParallelToY = true;
	else
		_isParallelToY = false;

	if (_isParallelToY)
	{
		if (_A[0].y() > _A[1].y()) Utils::swap(_A[0], _A[1]);
		if (_B[0].y() > _B[1].y()) Utils::swap(_B[0], _B[1]);
	}
	else
	{
		if (_A[0].x() > _A[1].x()) Utils::swap(_A[0], _A[1]);
		if (_B[0].x() > _B[1].x()) Utils::swap(_B[0], _B[1]);
	}
}
#pragma endregion
