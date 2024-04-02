#include "BombUpdater.h"
#include "GameServerBroadcaster.h"
#include "PlayerManager.h"
#include "Player.h"

#pragma region public
BombUpdater::BombUpdater()
{
	GameServerBroadcaster::onBombPlantReq(true, std::bind(&BombUpdater::onBombPlantReq, this, std::placeholders::_1));
}
BombUpdater::~BombUpdater()
{
	GameServerBroadcaster::onBombPlantReq(false, std::bind(&BombUpdater::onBombPlantReq, this, std::placeholders::_1));
}
#pragma endregion

#pragma region private
void BombUpdater::onBombPlantReq(int serial)
{
	Player* player = PlayerManager::getInstance().getPlayer(serial);
	if (player == nullptr)
		return;


}
void BombUpdater::explosion()
{
}
#pragma endregion