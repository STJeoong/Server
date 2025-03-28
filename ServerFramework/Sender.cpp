#include "pch.h"
#include "Sender.h"
#include "MemoryBlockPool.h"
#pragma region public
Sender::Sender(CompletionKey* completionKeys, int maxClient) : _completionKeys(completionKeys), _maxClient(maxClient)
{
	_sends = new OverlappedEx[maxClient];
	_sendMutexs = new std::mutex[maxClient];
	_thingsToSend = new std::queue<std::tuple<int, Size, char*>>[maxClient];
	for (int i = 0; i < maxClient; ++i)
	{
		memset(&_sends[i].o, 0, sizeof(OVERLAPPED));
		_sends[i].mode = IOMode::SEND;
		_sends[i].sessionIdx = i;
	}
}
Sender::~Sender()
{
	delete[] _sends;
	delete[] _sendMutexs;
	delete[] _thingsToSend;
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
	MemoryBlockPool::release((int)blockSize, block);
	_thingsToSend[idx].pop();
	if (!_thingsToSend[idx].empty())
		this->pendingSend(idx);
}
void Sender::onDisconnection(int idx)
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
	int ret = WSASend(ck.sock, &_sends[idx].wsa, 1, &bytes, 0, &_sends[idx].o, NULL);
	int error = WSAGetLastError();
	// 만약 WSASend 에러 났으면 바로 queue에서 pop?
	if ((ret == 0 && error == NO_ERROR) || (ret == SOCKET_ERROR && error == WSA_IO_PENDING))
		return;

	MemoryBlockPool::release((int)blockSize, block);
	_thingsToSend[idx].pop();
}
#pragma endregion