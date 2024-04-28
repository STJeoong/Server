#pragma once
#include "Collider2D.h"
#include "E_Layer.h"

class ContactFilter
{
public:
	static const int MAX_LAYER = 32;
	static bool shouldCollide(const Collider2D& colliderA, const Collider2D& colliderB);
	static void enableCollision(E_Layer layer1, E_Layer layer2);
	static void disableCollision(E_Layer layer1, E_Layer layer2);
	static void reset();
private:
	static bool s_layer[ContactFilter::MAX_LAYER][ContactFilter::MAX_LAYER];
};