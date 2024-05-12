#include "pch.h"
#include "Contact2D.h"

#pragma region public
#pragma endregion

#pragma region private
void Contact2D::onDestroy()
{
	_normalImpulse = 0.0f;
	_tangentImpulse = 0.0f;
	_depth = 0.0f;
	_rotationA = 0.0f;
	_rotationB = 0.0f;

	_bouncinessBias = 0.0f;
}
#pragma endregion