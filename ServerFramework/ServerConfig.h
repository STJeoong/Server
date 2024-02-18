#pragma once
#include <string>
struct ServerConfig
{
	std::string network;
	std::string ip;
	unsigned short port;
	unsigned short maxClient;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ServerConfig, network, ip, port, maxClient)