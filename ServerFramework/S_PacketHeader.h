#pragma once
#include <basetsd.h>


#pragma pack(push, 1)
struct S_PacketHeader
{
	UINT16 len;
	UINT16 initLen;
	UINT16 id;
	UINT8 option; // 패킷 암호화, 압축 여부 알아내기 위한 값
	UINT32 crc;
};
#pragma pack(pop)