#include "Server.h"

int main(int argc, char* argv[])
{
	Server::getInstance().init(argv);
	Server::getInstance().run();
	return 0;
}