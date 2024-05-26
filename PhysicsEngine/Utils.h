#pragma once

class Vector2D;
using Point2D = Vector2D;
class Utils
{
public:
	template<typename T>
	static T clamp(const T& v, const T& hi, const T& lo) { return v < lo ? lo : v > hi ? hi : v; }
	template<typename T>
	static void swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }
	static bool outOfEdge(const Point2D& projectedPoint, Point2D edgePoints[2]);
};