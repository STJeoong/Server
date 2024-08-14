#pragma once
#include <basetsd.h>
#include <google/protobuf/type.pb.h>
#include <string>
namespace Utils
{
	void send(int engineID, int serial, UINT16 packetID, UINT8 option, const google::protobuf::Message& message);
	void send(int engineID, int serial, UINT16 packetID, UINT8 option);
	std::string eraseFileExtensionName(const std::string& fileName);
}