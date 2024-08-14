#include "Utils.h"
#include <Engine.h>
#include <MemoryBlockPool.h>
#include <S_PacketHeader.h>

void Utils::send(int engineID, int serial, UINT16 packetID, UINT8 option, const google::protobuf::Message& message)
{
	char* block = MemoryBlockPool::get((int)Size::_2048);
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(block);

	header->id = packetID;
	header->option = option;
	header->initLen = sizeof(S_PacketHeader) + (UINT16)message.ByteSizeLong();
	header->len = sizeof(S_PacketHeader) + (UINT16)message.ByteSizeLong();

	message.SerializeToArray(block + sizeof(S_PacketHeader), (int)Size::_2048 - (int)sizeof(S_PacketHeader));
	Engine::send(engineID, serial, Size::_2048, block);
}
void Utils::send(int engineID, int serial, UINT16 packetID, UINT8 option)
{
	char* block = MemoryBlockPool::get((int)Size::_256);
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(block);

	header->id = packetID;
	header->option = option;
	header->initLen = sizeof(S_PacketHeader);
	header->len = sizeof(S_PacketHeader);

	Engine::send(engineID, serial, Size::_256, block);
}
std::string Utils::eraseFileExtensionName(const std::string& fileName)
{
	std::string ret = fileName;
	size_t idx = ret.find('.');
	return ret.substr(0, idx);
}