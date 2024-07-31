#include <Engine.h>
#include <fstream>
#include "E_EngineType.h"
#include "LobbyServerBroadcaster.h"
#include "DBClientBroadcaster.h"
#include "MatchClientBroadcaster.h"
#include "LobbyServerHandler.h"
#include "DBClientHandler.h"
#include "MatchClientHandler.h"

static void setEngine()
{
	std::ifstream f("config.json");
	json data = json::parse(f);
	S_ServerConfig lobbyConfig = {};
	S_ServerConfig dbConfig = {};
	S_ServerConfig matchConfig = {};

	data["server"].get_to(lobbyConfig);
	data["db_client"].get_to(dbConfig);
	data["match_client"].get_to(matchConfig);

	Engine::addEngine((int)E_EngineType::LOBBY_SERVER, lobbyConfig, new LobbyServerBroadcaster());
	Engine::addEngine((int)E_EngineType::DB_CLIENT, dbConfig, new DBClientBroadcaster());
	Engine::addEngine((int)E_EngineType::MATCH_CLIENT, matchConfig, new MatchClientBroadcaster());
}

int main(int argc, char* argv[])
{
	Engine::setLogFolder(argv[0]);
	Engine::setServerMode(E_ServerMode::PASSIVE_MODE);
	setEngine();

	DBClientHandler dbHandler;
	LobbyServerHandler lobbyHandler;
	MatchClientHandler matchHandler;

	Engine::run();
}