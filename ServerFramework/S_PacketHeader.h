#pragma once
#include <basetsd.h>


#pragma pack(push, 1)
struct S_PacketHeader
{
	UINT16 len;
	UINT16 initLen;
	UINT16 id;
	UINT8 option; // ��Ŷ ��ȣȭ, ���� ���� �˾Ƴ��� ���� ��
	UINT32 crc;
};
#pragma pack(pop)