#include <Engine.h>
#include "E_EngineType.h"
#include "MMOServerBroadcaster.h"
#include "Game.h"

static void setEngine()
{
	S_ServerConfig config = {};
	config.ip = "172.30.1.26";
	config.port = 9090;
	config.network = "server";
	config.maxClient = 100;

	Engine::addEngine((int)E_EngineType::MMO_SERVER, config, new MMOServerBroadcaster());
}
#include <vector>
int main(int argc, char* argv[])
{
	DAT dat;
	Engine::setLogFolder(argv[0]);
	Engine::init();
	Engine::setServerMode(E_ServerMode::PASSIVE_MODE);
	setEngine();
	Game::init();
	Engine::run();
	Engine::end();
}