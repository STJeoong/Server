#pragma once
#include "Vector2D.h"
class Collision2D;
class Contact2D;
class ContactGenerator
{
public:
	ContactGenerator(Collision2D& collision, const Vector2D& normal);
	int contactNum() const { return _contactNum; }
	Contact2D* generate(Collision2D& collision, int idx);
private:
	Contact2D* EdgeToEdge(Collision2D& collision, int idx);
	Contact2D* EdgeToPoint(Collision2D& collision);
	Contact2D* PointToPoint(Collision2D& collision);
	Contact2D* generateContact(Collision2D& collsion, bool isEdgeA, bool isEdgeB, int vertexIdx);
	void preProcess();
	

	int _contactNum = 0;
	bool _isVertexA = false;
	bool _isVertexB = false;
	bool _isParallelToY = false;
	Vector2D _normal;
	Point2D _A[2];
	Point2D _B[2];
};