#pragma once
#include <WinSock2.h>

enum class IOMode
{
	ACCEPT,
	CONNECT,
	RECV,
	SEND
};

struct OverlappedEx
{
	OVERLAPPED o;
	WSABUF wsa;
	IOMode mode;
	int sessionIdx;
};
struct CompletionKey
{
	int id;
	SOCKET sock; // 클라이언트 소켓
};