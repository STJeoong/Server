#pragma once
#include "OverlappedEx.h"
class Receiver
{
	static const UINT16 RECV_BUF_SIZE = 1024;
public:
	Receiver(CompletionKey* completionKeys, int maxClient);
	~Receiver();
	char* getBuf(int idx);
	void onConnect(int idx);
	void onRecv(int idx);
private:
	void pendingRecv(int idx);

	int _maxClient = 0;
	CompletionKey* _completionKeys = nullptr;
	OverlappedEx* _recvs = nullptr;
	char** _recvBufs = nullptr;
};

