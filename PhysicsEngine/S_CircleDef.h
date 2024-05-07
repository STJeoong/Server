#pragma once
#include "Vector2D.h"

struct S_CircleDef
{
	Point2D offset;
	float density = 1.0f;
	float friction = 0.4f;
	float bounciness = 0.0f;
	float bouncinessThreshold = 1.0f;
	bool isTrigger = false;
	float radius = 1.0f;
};