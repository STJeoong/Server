#pragma once
#include <mutex>
#include <queue>
#include "Define.h"
#include "OverlappedEx.h"

class Sender
{
public:
	Sender(CompletionKey* completionKeys);
	~Sender() = default;
	Sender(const Sender& obj) = delete;
	Sender(Sender&& obj) = delete;
	Sender& operator=(const Sender& obj) = delete;
	Sender& operator=(Sender&& obj) = delete;

	void send(int to, Size blockSize, int len, char* block);
	void onSend(int idx, DWORD bytes);
	void onCloseConnection(int idx);
private:
	void pendingSend(int completionKeyId);


	CompletionKey* _completionKeys;
	OverlappedEx _sends[MAX_CLIENT];
	std::mutex _sendMutexs[MAX_CLIENT];
	std::queue<std::tuple<int, Size, char*>> _thingsToSend[MAX_CLIENT];
};

