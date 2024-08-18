#pragma once
#include "E_Layer.h"

class LayerFilter
{
public:
	static const int MAX_LAYER = 63;
	static void init();
	static bool detectable(E_Layer layer1, E_Layer layer2) { return s_table[(int)layer1][(int)layer2]; }
	static void enable(E_Layer layer1, E_Layer layer2);
	static void disable(E_Layer layer1, E_Layer layer2);

	static bool s_table[LayerFilter::MAX_LAYER + 1][LayerFilter::MAX_LAYER + 1];
};