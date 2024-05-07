#pragma once
#include "Vector2D.h"

struct S_BoxDef
{
	S_BoxDef() : halfSize(0.5f, 0.5f) {}
	Point2D offset;
	float density = 1.0f;
	float friction = 0.4f;
	float bounciness = 0.0f;
	float bouncinessThreshold = 1.0f;
	bool isTrigger = false;
	Vector2D halfSize; // 회전이 적용되지 않은 값
};