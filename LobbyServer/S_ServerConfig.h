#pragma once
#include <string>
#include <nlohmann/json.hpp>
struct S_ServerConfig
{
	std::string network;
	std::string ip;
	unsigned short port;
	unsigned short maxClient;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_ServerConfig, network, ip, port, maxClient)