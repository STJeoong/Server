#pragma once
#include "pch.h"

// 상수 모음
#define MAX_USER_ACCEPT 65535 //서버에서 받아들일 수 있는 최대 소켓수
#define DEFALUT_USER_ACCEPT 5000 //디폴트 유저 접속 허용 수.. 5000

#define PACKET_OPTION_DATACRC 0x00000001
#define PACKET_OPTION_COMPRESS 0x00000002
#define PACKET_OPTION_ENCRYPTION 0x00000004
#define PACKET_OPTION_CRC_DEFAULT 0x00000008

// 크기 정의
enum class Size : UINT16
{
	_128 = 128,
	_256 = 256,
	_1024 = 1024,
	_8192 = 8192
};


// 매크로 모음
#ifdef UNICODE
#define tstring std::wstring
#define tcout wcout
#else
#define tstring std::string
#define tcout cout
#endif
