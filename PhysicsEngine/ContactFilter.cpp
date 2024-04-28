#include "pch.h"
#include "ContactFilter.h"
#include "GameObject.h"

bool ContactFilter::shouldCollide(const Collider2D& colliderA, const Collider2D& colliderB)
{
	int layer1 = (int)colliderA.gameObject()->layer();
	int layer2 = (int)colliderB.gameObject()->layer();
	return s_layer[layer1][layer2];
}
void ContactFilter::enableCollision(E_Layer layer1, E_Layer layer2)
{
	s_layer[(int)layer1][(int)layer2] = true;
	s_layer[(int)layer2][(int)layer1] = true;
}
void ContactFilter::disableCollision(E_Layer layer1, E_Layer layer2)
{
	s_layer[(int)layer1][(int)layer2] = false;
	s_layer[(int)layer2][(int)layer1] = false;
}
void ContactFilter::reset()
{
	for (int i = 0; i < ContactFilter::MAX_LAYER; ++i)
		for (int j = 0; j < ContactFilter::MAX_LAYER; ++j)
			s_layer[i][j] = true;
}
bool ContactFilter::s_layer[ContactFilter::MAX_LAYER][ContactFilter::MAX_LAYER];