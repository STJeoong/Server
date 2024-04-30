#pragma once
#include <vector>
#include <queue>
#include <tuple>
#include "Vector2D.h"
#include "Collision2D.h"
class Polytope
{
public:
	Polytope(const Collision2D& collision, const std::vector<Point2D>& points);
	float depth() const { return _depth; }
	const Vector2D& normal() const { return _normal; }
private:
	void initPQ();
	void expand(const Collision2D& collision);
	// �� ���� ������ ������ ���� ������ �Ÿ�
	float computeDistance(const Point2D& A, const Point2D& B);

	std::vector<Point2D> _points;
	std::priority_queue<std::tuple<float, size_t, size_t>, std::vector<std::tuple<float, size_t, size_t>>,
		std::greater<std::tuple<float, size_t, size_t>>> _pq;
	float _depth = 0.0f;
	Vector2D _normal;
};