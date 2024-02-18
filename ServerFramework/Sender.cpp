#include "pch.h"
#include "Sender.h"
#include "MemoryBlockPoolManager.h"
#pragma region public
Sender::Sender(CompletionKey* completionKeys) : _completionKeys(completionKeys)
{
	for (int i = 0; i < MAX_CLIENT; ++i)
	{
		memset(&_sends[i].o, 0, sizeof(OVERLAPPED));
		_sends[i].mode = IOMode::SEND;
		_sends[i].sessionIdx = i;
	}
}
void Sender::send(int to, Size blockSize, int len, char* block)
{
	std::lock_guard<std::mutex> lock(_sendMutexs[to]);
	_thingsToSend[to].push({ len, blockSize, block });
	if (_thingsToSend[to].size() == 1)
		this->pendingSend(to);
}
void Sender::onSend(int idx, DWORD bytes)
{
	int len;
	Size blockSize;
	char* block;

	std::lock_guard<std::mutex> lock(_sendMutexs[idx]);
	std::tie(len, blockSize, block) = _thingsToSend[idx].front();
	MemoryBlockPoolManager::getInstance().release(blockSize, block);
	_thingsToSend[idx].pop();
	if (!_thingsToSend[idx].empty())
		this->pendingSend(idx);
}
void Sender::onCloseConnection(int idx)
{
	std::lock_guard<std::mutex> lock(_sendMutexs[idx]);
	while (!_thingsToSend[idx].empty())
		_thingsToSend[idx].pop();
}
#pragma endregion

#pragma region private
void Sender::pendingSend(int idx)
{
	DWORD bytes;
	CompletionKey& ck = _completionKeys[idx];
	int len;
	Size blockSize;
	char* block;

	std::tie(len, blockSize, block) = _thingsToSend[idx].front();
	_sends[idx].wsa.buf = block;
	_sends[idx].wsa.len = len;
	BOOL ret = WSASend(ck.sock, &_sends[idx].wsa, 1, &bytes, 0, &_sends[idx].o, NULL);
	// 만약 WSASend 에러 났으면 바로 queue에서 pop?
	if (ret == FALSE && WSAGetLastError() != WSA_IO_PENDING)
	{
		MemoryBlockPoolManager::getInstance().release(blockSize, block);
		_thingsToSend[idx].pop();
	}
}
#pragma endregion