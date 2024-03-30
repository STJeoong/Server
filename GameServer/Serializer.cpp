#include "pch.h"
#include "Serializer.h"
#include <MemoryBlockPool.h>
#include <S_PacketHeader.h>

#pragma region public
std::pair<Size, char*> Serializer::serialize(S_PacketAttr attr)
{
	char* block = MemoryBlockPool::get(Size::_256);
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(block);

	header->id = attr.packetID;
	header->option = attr.option;
	header->initLen = sizeof(S_PacketHeader);
	header->len = sizeof(S_PacketHeader);

	return { Size::_256, block };
}
std::pair<Size, char*> Serializer::serialize(S_PacketAttr attr, const google::protobuf::Message& message)
{
	char* block = MemoryBlockPool::get(Size::_1024);
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(block);

	header->id = attr.packetID;
	header->option = attr.option;
	header->initLen = sizeof(S_PacketHeader) + (UINT16)message.ByteSizeLong();
	header->len = sizeof(S_PacketHeader) + (UINT16)message.ByteSizeLong();

	message.SerializeToArray(block + sizeof(S_PacketHeader), (int)Size::_1024 - (int)sizeof(S_PacketHeader));
	return { Size::_1024, block };
}
#pragma endregion

#pragma region private
#pragma endregion