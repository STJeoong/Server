#include "pch.h"
#include "Utils.h"
#include "Vector2D.h"

bool Utils::outOfEdge(const Point2D& projectedPoint, Point2D edgePoints[2])
{
	static const float TOLERANCE = 0.0001f;
	if (projectedPoint.x() < std::min(edgePoints[0].x(), edgePoints[1].x()) - TOLERANCE ||
		projectedPoint.x() > std::max(edgePoints[0].x(), edgePoints[1].x()) + TOLERANCE ||
		projectedPoint.y() < std::min(edgePoints[0].y(), edgePoints[1].y()) - TOLERANCE ||
		projectedPoint.y() > std::max(edgePoints[0].y(), edgePoints[1].y()) + TOLERANCE)
		return true;
	return false;
}