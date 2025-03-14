#include <Engine.h>
#include <fstream>
#include "E_EngineType.h"
#include "MMOServerBroadcaster.h"
#include "DBClientBroadcaster.h"
#include "Game.h"
#include <DbgHelp.h>
#include <map>

#pragma comment(lib, "DbgHelp.lib")
static void setEngine()
{
	std::ifstream fstream("C:/Users/taejeong/source/repos/ServerFramework/MMOServer/data/config.json");
	json j = json::parse(fstream);
	std::map<std::string, S_ServerConfig> mp;
	j.get_to(mp);

	Engine::addEngine((int)E_EngineType::MMO_SERVER, mp["mmo"], new MMOServerBroadcaster());
	Engine::addEngine((int)E_EngineType::DB_CLIENT, mp["db"], new DBClientBroadcaster());
}
static LONG WINAPI createDump(EXCEPTION_POINTERS* exceptPointers)
{
	HANDLE hFile = CreateFile(L"C:/Users/taejeong/source/repos/ServerFramework/MMOServer/x64/dumpfile.dmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION mdei;		
		mdei.ThreadId = GetCurrentThreadId();
		mdei.ExceptionPointers = exceptPointers;
		mdei.ClientPointers = FALSE;

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &mdei, NULL, NULL);

		CloseHandle(hFile);
	}
	return EXCEPTION_EXECUTE_HANDLER;
}
int main(int argc, char* argv[])
{
	SetUnhandledExceptionFilter(createDump);

	Engine::setLogFolder(argv[0]);
	Engine::init();
	Engine::setServerMode(E_ServerMode::PASSIVE_MODE);
	setEngine();
	Game::init();
	Engine::run();
	Engine::end();
}