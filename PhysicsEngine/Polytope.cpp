#include "pch.h"
#include "Polytope.h"
#include "Collider2D.h"
#include "Line.h"

#pragma region public
void Polytope::init(const Collision2D& collision, const std::vector<Point2D>& points, const std::vector<std::pair<Point2D, Point2D>>& sources)
{
	_points.clear();
	_sources.clear();
	_distances.clear();
	_points = points;
	_sources = sources;

	if (_points.size() < 3)
		this->setMinimumPoints(collision);
	this->initDistances();
	this->expand(collision);
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
			_sources.push_back({ pointFromA, pointFromB });
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
		if (supportVec == Vector2D{ 0.0f, 0.0f })
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
		if (A == minkowskiPoint || B == minkowskiPoint || line.isPointOnTheLine(minkowskiPoint) || i == Polytope::MAX_ITERATION)
		{
			_normal = supportVec;
			this->computeClosestPoints(colliderA, colliderB, idxA, idxB);
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
		_sources.push_back({ pointFromA, pointFromB });
		++i;
	}
}
void Polytope::computeClosestPoints(Collider2D* colliderA, Collider2D* colliderB, size_t idxA, size_t idxB)
{
	Point2D A[2] = { _sources[idxA].first, _sources[idxB].first };
	Point2D B[2] = { _sources[idxA].second, _sources[idxB].second };

	// check if it contacts to collider's edge
	bool isVertexA = A[0] == A[1];
	if (colliderA->isCircle())
		isVertexA = true;
	bool isVertexB = B[0] == B[1];
	if (colliderB->isCircle())
		isVertexB = true;

	if (!isVertexA && !isVertexB) // 면 대 면 접촉
	{
		bool isParallelToY = false;
		if (std::abs(A[0].x() - A[1].x()) < FLT_EPSILON) // y축에 평행한 면에 접촉
		{
			isParallelToY = true;
			// swap
			float tmp = A[0].x();
			A[0].x(A[0].y());
			A[0].y(tmp);
			tmp = A[1].x();
			A[1].x(A[1].y());
			A[1].y(tmp);

			tmp = B[0].x();
			B[0].x(B[0].y());
			B[0].y(tmp);
			tmp = B[1].x();
			B[1].x(B[1].y());
			B[1].y(tmp);
		}

		// get x coordinate
		if (A[0].x() > A[1].x()) Utils::swap(A[0], A[1]);
		if (B[0].x() > B[1].x()) Utils::swap(B[0], B[1]);
		float leftX = std::max(A[0].x(), B[0].x());
		float rightX = std::min(A[1].x(), B[1].x());
		float result = (leftX + rightX) * 0.5f; // 중간점 선택
		_contactA.x(result);
		_contactB.x(result);

		// get each collider's y coordinate from x
		_contactA.y(((A[1].x() - result) / (A[1].x() - A[0].x())) * A[0].y() + ((result - A[0].x()) / (A[1].x() - A[0].x())) * A[1].y());
		_contactB.y(((B[1].x() - result) / (B[1].x() - B[0].x())) * B[0].y() + ((result - B[0].x()) / (B[1].x() - B[0].x())) * B[1].y());
		if (isParallelToY)
		{
			float tmp = _contactA.x();
			_contactA.x(_contactA.y());
			_contactA.y(tmp);

			tmp = _contactB.x();
			_contactB.x(_contactB.y());
			_contactB.y(tmp);
		}
	}
	else // 면 대 점 접촉
	{
		if (isVertexA && isVertexB) // (사각형 꼭짓점, 원), (원, 원) 충돌일 때 가능
		{
			_contactA = A[0];
			_contactB = B[0];
			if (colliderA->isCircle())
				_normal = A[0] - colliderA->position();
			else if (colliderB->isCircle())
				_normal = colliderB->position() - B[0];
		}
		else if (isVertexA) // A는 점 접촉, B는 면 접촉
		{
			_contactA = A[0];

			// project A[0] to B's edge
			// ax + by + c = 0
			Line line(B[1], B[0]);
			float a = line.a();
			float b = line.b();
			float c = line.c();
			float k = -1.0f * (a * A[0].x() + b * A[0].y() + c) / (a * a + b * b);

			_contactB.x(A[0].x() + a * k);
			_contactB.y(A[0].y() + b * k);
		}
		else // A는 면 접촉, B는 점 접촉
		{
			_contactB = B[0];

			// project B[0] to A's edge
			// ax + by + c = 0
			Line line(A[1], A[0]);
			float a = line.a();
			float b = line.b();
			float c = line.c();
			float k = -1.0f * (a * B[0].x() + b * B[0].y() + c) / (a * a + b * b);

			_contactA.x(B[0].x() + a * k);
			_contactA.y(B[0].y() + b * k);
		}
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