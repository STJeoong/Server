#pragma once
#include "E_BodyType.h"

struct S_RigidDef
{
	E_BodyType type = E_BodyType::DYNAMIC;
	float gravityScale = 1.0f;
	bool awake = true;
	bool fixedRotation = false;
	bool autoSleep = true;
};