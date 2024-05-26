#include "pch.h"
#include "Polytope.h"
#include "Collider2D.h"
#include "Line.h"
#include "ObjectPool.h"
#include "Contact2D.h"
#include "GameObject.h"

#pragma region public
void Polytope::init(const Collision2D& collision, const std::vector<Point2D>& points)
{
	_points.clear();
	_distances.clear();
	_points = points;

	if (_points.size() < 3)
		this->setMinimumPoints(collision);
	this->initDistances();
}
void Polytope::expand(const Collision2D& collision)
{
	Collider2D* colliderA = collision.colliderA();
	Collider2D* colliderB = collision.colliderB();
	int i = 0;
	while (true)
	{
		float distance;
		size_t idxA, idxB;
		this->getMinDistance(distance, idxA, idxB);
		const Point2D& A = _points[idxA];
		const Point2D& B = _points[idxB];
		Vector2D AB = B - A;
		Vector2D AO = A * -1;
		Vector2D supportVec = Vector2D::cross(Vector2D::cross(AO, AB), AB); // FIX: supportVec가 {0.0f, 0.0f}였음
		if (supportVec == Vector2D(0.0f, 0.0f))
		{
			Point2D barycentric = { 0.0f,0.0f };
			for (int i = 0; i < _points.size(); ++i)
				barycentric += _points[i];
			barycentric *= 1.0f / _points.size();
			AO = barycentric - A;
			supportVec = Vector2D::cross(Vector2D::cross(AO, AB), AB);
		}

		Point2D pointFromA = colliderA->computeSupportPoint(supportVec);
		Point2D pointFromB = colliderB->computeSupportPoint(supportVec * -1);
		Point2D minkowskiPoint = pointFromA - pointFromB;
		Line line(A, B);
		bool onTheLine = line.isPointOnTheLine(minkowskiPoint);
		if (A == minkowskiPoint || B == minkowskiPoint || onTheLine || i == Polytope::MAX_ITERATION)
		{
			_normal = supportVec.normalized();
			return;
		}

		{
			Line line(A, minkowskiPoint);
			float computedValue = line.squaredDistanceFrom({ 0.0f, 0.0f });
			_distances.push_back({ false, computedValue, idxA, _points.size() });
		}
		{
			Line line(B, minkowskiPoint);
			float computedValue = line.squaredDistanceFrom({ 0.0f, 0.0f });
			_distances.push_back({ false, computedValue, idxB, _points.size() });
		}
		_points.push_back(minkowskiPoint);
		++i;
	}
}
#pragma endregion

#pragma region private
// 0-simplex가 원점이거나, 1-simplex가 원점을 지나는 선일 때
void Polytope::setMinimumPoints(const Collision2D& collision)
{
	Collider2D* colliderA = collision.colliderA();
	Collider2D* colliderB = collision.colliderB();

	Vector2D supportVectorCandidates[4] = { {1.0f,0.0f}, {-1.0f,0.0f}, {0.0f,1.0f}, {0.0f,-1.0f} };
	while (_points.size() < 3)
	{
		for (int i = 0; i < 4; ++i)
		{
			bool valid = true;
			Point2D pointFromA = colliderA->computeSupportPoint(supportVectorCandidates[i]);
			Point2D pointFromB = colliderB->computeSupportPoint(supportVectorCandidates[i] * -1);
			Point2D newPoint = pointFromA - pointFromB;
			for (int j = 0; j < _points.size(); ++j)
				if (newPoint == _points[j])
				{
					valid = false;
					break;
				}
			if (!valid)
				continue;
			if (_points.size() == 2)
			{
				Line line(_points[0], _points[1]);
				if (line.isPointOnTheLine(newPoint))
					continue;
			}
			_points.push_back(newPoint);
			break;
		}
	}
}
void Polytope::initDistances()
{
	for (size_t i = 0; i < _points.size(); ++i)
	{
		Line line(_points[i], _points[(i + 1) % _points.size()]);
		// TODO : 값이 너무 커질 수 있나?
		float distance = line.squaredDistanceFrom({ 0.0f, 0.0f });
		_distances.push_back({ false, distance, i, (i + 1) % _points.size() });
		//_pq.push({ distance, i, (i + 1) % _points.size() });
	}
}
void Polytope::getMinDistance(float& distance, size_t& idxA, size_t& idxB)
{
	int wh = 0;
	for (int i = 0; i < _distances.size(); ++i)
		if (!std::get<0>(_distances[i]))
		{
			distance = std::get<1>(_distances[i]);
			idxA = std::get<2>(_distances[i]);
			idxB = std::get<3>(_distances[i]);
			wh = i;
			break;
		}
	
	for (int i = 0; i < _distances.size(); ++i)
		if (!std::get<0>(_distances[i]) && distance > std::get<1>(_distances[i]))
		{
			distance = std::get<1>(_distances[i]);
			idxA = std::get<2>(_distances[i]);
			idxB = std::get<3>(_distances[i]);
			wh = i;
		}
	std::get<0>(_distances[wh]) = true;
}
#pragma endregion