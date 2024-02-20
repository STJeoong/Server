#pragma once
#include <mutex>
#include <queue>
#include "Define.h"
#include "OverlappedEx.h"

class Sender
{
public:
	Sender(CompletionKey* completionKeys, int maxClient);
	~Sender();
	Sender(const Sender& obj) = delete;
	Sender(Sender&& obj) = delete;
	Sender& operator=(const Sender& obj) = delete;
	Sender& operator=(Sender&& obj) = delete;

	void send(int to, Size blockSize, int len, char* block);
	void onSend(int idx, DWORD bytes);
	void onDisconnection(int idx);
private:
	void pendingSend(int completionKeyId);


	int _maxClient = 0;
	CompletionKey* _completionKeys;
	OverlappedEx* _sends;
	std::mutex* _sendMutexs;
	std::queue<std::tuple<int, Size, char*>>* _thingsToSend;
};

