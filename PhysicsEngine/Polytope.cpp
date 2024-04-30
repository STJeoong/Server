#include "pch.h"
#include "Polytope.h"
#include "Collider2D.h"

#pragma region public
Polytope::Polytope(const Collision2D& collision, const std::vector<Point2D>& points) : _points(points)
{
	this->initPQ();
	this->expand(collision);
}
#pragma endregion

#pragma region private
void Polytope::initPQ()
{
	for (size_t i = 0; i < _points.size(); ++i)
	{
		float distance = this->computeDistance(_points[i], _points[(i + 1) % _points.size()]);
		_pq.push({ distance, i, (i + 1) % _points.size() });
	}
}
void Polytope::expand(const Collision2D& collision)
{
	Collider2D* colliderA = collision.colliderA();
	Collider2D* colliderB = collision.colliderB();
	while (!_pq.empty())
	{
		float distance;
		size_t idxA, idxB;
		std::tie(distance, idxA, idxB) = _pq.top();
		_pq.pop();

		const Point2D& A = _points[idxA];
		const Point2D& B = _points[idxB];
		Vector2D AB = B - A;
		Vector2D AO = A * -1;
		Vector2D supportVec = Vector2D::cross(Vector2D::cross(AO, AB), AB);
		Point2D minkowskiPoint = colliderA->computeSupportPoint(supportVec) - colliderB->computeSupportPoint(supportVec * -1);
		if (A == minkowskiPoint || B == minkowskiPoint)
		{
			_depth = distance;
			_normal = supportVec;
			return;
		}

		_points.push_back(minkowskiPoint);
		{
			float computedValue = this->computeDistance(A, minkowskiPoint);
			_pq.push({ computedValue, idxA, _points.size() - 1 });
		}
		{
			float computedValue = this->computeDistance(B, minkowskiPoint);
			_pq.push({ computedValue, idxB, _points.size() - 1 });
		}
	}
}
float Polytope::computeDistance(const Point2D& A, const Point2D& B)
{
	if (A == B) return A.len();
	// ax + by + c = 0 직선 방정식 만들기
	Vector2D AB = B - A;
	return std::abs(AB.x() * A.y() - AB.y() * A.x()) / std::sqrtf(AB.x() * AB.x() + AB.y() * AB.y());
}
#pragma endregion