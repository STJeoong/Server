#include "Server.h"

int main(int argc, char* argv[])
{
	Server::getInstance().init(argv[0]);
	Server::getInstance().run();
	return 0;
}