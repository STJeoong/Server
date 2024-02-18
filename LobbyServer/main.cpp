#pragma comment(lib, "ServerFramework.lib")
#include <Engine.h>
#include "Server.h"

int main(int argc, char* argv[])
{
	Engine::getInstance().init(argv[0], new Server);
	Engine::getInstance().start();

	getchar();

	Engine::getInstance().shutdown();
}