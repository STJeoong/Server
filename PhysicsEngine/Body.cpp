#include "pch.h"
#include "Body.h"

#pragma region public
void Body::mass(float m)
{
	_mass = m;
	if (_mass < FLT_MAX)
		_invMass = 1.0f / _mass;
	else if (_mass == FLT_MAX)
		_invMass = 0.0f;
}
#pragma endregion