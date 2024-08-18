#include "LayerFilter.h"

#pragma region public
void LayerFilter::init()
{
	// TODO : 설정 파일로?
	LayerFilter::enable(E_Layer::AOI, E_Layer::PLAYER_OBJ);
	LayerFilter::enable(E_Layer::AOI, E_Layer::MONSTER_OBJ);
	LayerFilter::enable(E_Layer::AOI, E_Layer::PROJECTILE_OBJ);
	LayerFilter::enable(E_Layer::AOI, E_Layer::ITEM_OBj);
	
	LayerFilter::enable(E_Layer::MONSTER_AGGRESSIVE, E_Layer::PLAYER_OBJ);
}
void LayerFilter::enable(E_Layer layer1, E_Layer layer2)
{
	s_table[(int)layer1][(int)layer2] = true;
	s_table[(int)layer2][(int)layer1] = true;
}
void LayerFilter::disable(E_Layer layer1, E_Layer layer2)
{
	s_table[(int)layer1][(int)layer2] = false;
	s_table[(int)layer2][(int)layer1] = false;
}
#pragma endregion

#pragma region private
#pragma endregion

bool LayerFilter::s_table[LayerFilter::MAX_LAYER + 1][LayerFilter::MAX_LAYER + 1] = {};