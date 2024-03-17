#pragma once
#include "pch.h"

// ��� ����
#define MAX_USER_ACCEPT 65535 //�������� �޾Ƶ��� �� �ִ� �ִ� ���ϼ�
#define DEFALUT_USER_ACCEPT 5000 //����Ʈ ���� ���� ��� ��.. 5000

#define PACKET_OPTION_DATACRC 0x00000001
#define PACKET_OPTION_COMPRESS 0x00000002
#define PACKET_OPTION_ENCRYPTION 0x00000004
#define PACKET_OPTION_CRC_DEFAULT 0x00000008

// ũ�� ����
enum class Size : UINT16
{
	_128 = 128,
	_256 = 256,
	_1024 = 1024,
	_8192 = 8192
};


// ��ũ�� ����
#ifdef UNICODE
#define tstring std::wstring
#define tcout wcout
#else
#define tstring std::string
#define tcout cout
#endif
