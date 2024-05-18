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
	void init(const Collision2D& collision, const std::vector<Point2D>& points, const std::vector<std::pair<Point2D, Point2D>>& sources);
	const Vector2D& normal() const { return _normal; } // A to B
	const Point2D& contactA() const { return _contactA; }
	const Point2D& contactB() const { return _contactB; }
private:
	void setMinimumPoints(const Collision2D& collision);
	void initDistances();
	void expand(const Collision2D& collision);
	void computeClosestPoints(Collider2D* colliderA, Collider2D* colliderB, size_t idxA, size_t idxB);
	void getMinDistance(float& distance, size_t& idxA, size_t& idxB);

	std::vector<Point2D> _points;
	std::vector<std::pair<Point2D, Point2D>> _sources;
	std::vector<std::tuple<bool, float, size_t, size_t>> _distances;
	Vector2D _normal;
	Point2D _contactA;
	Point2D _contactB;
};