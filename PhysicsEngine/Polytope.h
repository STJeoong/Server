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
	void init(const Collision2D& collision, const std::vector<Point2D>& points);
	void expand(const Collision2D& collision);
	const Vector2D& normal() const { return _normal; }
private:
	void setMinimumPoints(const Collision2D& collision);
	void initDistances();
	void getMinDistance(float& distance, size_t& idxA, size_t& idxB);

	std::vector<Point2D> _points;
	std::vector<std::tuple<bool, float, size_t, size_t>> _distances;
	Vector2D _normal;
};