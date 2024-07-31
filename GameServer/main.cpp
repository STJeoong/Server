#include <Engine.h>
#include "E_EngineType.h"
#include "GameServerBroadcaster.h"
#include "GameManager.h"
#include "PlayerMoveUpdater.h"

static void setEngine(char* argv[])
{
	S_ServerConfig config{};
	config.network = "iocp_server";
	config.maxClient = 2;
	config.ip = argv[1];
	config.port = atoi(argv[2]);

	Engine::addEngine((int)E_EngineType::GAME_SERVER, config, new GameServerBroadcaster());
}

int main(int argc, char* argv[])
{
	Engine::setLogFolder(argv[0]);
	Engine::setServerMode(E_ServerMode::PASSIVE_MODE);
	setEngine(argv);

	GameManager gameManager;
	PlayerMoveUpdater moveUpdater;
	Engine::run();
	return 0;
}