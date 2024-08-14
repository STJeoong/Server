#include "Monster.h"
#include "MMOServerBroadcaster.h"

#pragma region public
void Monster::init()
{
	MMOServerBroadcaster::onUpdate += Monster::update;
}
#pragma endregion

#pragma region private
void Monster::update()
{
}
#pragma endregion