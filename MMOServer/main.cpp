#include <Engine.h>
#include <fstream>
#include "E_EngineType.h"
#include "MMOServerBroadcaster.h"
#include "Game.h"

static void setEngine()
{
	std::ifstream fstream("C:/Users/taejeong/source/repos/ServerFramework/MMOServer/data/config.json");
	json j = json::parse(fstream);
	S_ServerConfig config;
	j.get_to(config);

	Engine::addEngine((int)E_EngineType::MMO_SERVER, config, new MMOServerBroadcaster());
}
int main(int argc, char* argv[])
{
	Engine::setLogFolder(argv[0]);
	Engine::init();
	Engine::setServerMode(E_ServerMode::PASSIVE_MODE);
	setEngine();
	Game::init();
	Engine::run();
	Engine::end();
}