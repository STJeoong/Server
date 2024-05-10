#pragma once
#include <vector>
#include <queue>
#include <tuple>
#include "Vector2D.h"
#include "Collision2D.h"
class Polytope
{
	static const int MAX_ITERATION = 20;
public:
	Polytope(const Collision2D& collision, const std::vector<Point2D>& points, const std::vector<std::pair<Point2D, Point2D>>& sources);
	const Vector2D& normal() const { return _normal; } // A to B
	const Point2D& contactA() const { return _contactA; }
	const Point2D& contactB() const { return _contactB; }
	float depth() const { return _depth; }
private:
	void initPQ();
	void expand(const Collision2D& collision);
	void computeClosestPoints(size_t idxA, size_t idxB);

	std::vector<Point2D> _points;
	std::vector<std::pair<Point2D, Point2D>> _sources;
	std::priority_queue<std::tuple<float, size_t, size_t>, std::vector<std::tuple<float, size_t, size_t>>,
		std::greater<std::tuple<float, size_t, size_t>>> _pq;
	Vector2D _normal;
	Point2D _contactA;
	Point2D _contactB;
	float _depth;
};