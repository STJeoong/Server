#include <Engine.h>
#include <fstream>
#include "E_EngineType.h"
#include "MatchServerBroadcaster.h"
#include "MatchServerHandler.h"

static void setEngine()
{
	std::ifstream f("match_config.json");
	json data = json::parse(f);
	S_ServerConfig config;

	data["server"].get_to(config);

	Engine::addEngine((int)E_EngineType::MATCH_SERVER, config, new MatchServerBroadcaster());
}

int main(int argc, char* argv[])
{
	Engine::setLogFolder(argv[0]);
	Engine::setServerMode(E_ServerMode::PASSIVE_MODE);
	setEngine();

	MatchServerHandler handler;
	Engine::runEngine();
	Engine::runServer();
	return 0;
}