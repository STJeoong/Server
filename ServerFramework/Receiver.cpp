#include "pch.h"
#include "Receiver.h"


#pragma region public
Receiver::Receiver(CompletionKey* completionKeys, int maxClient) : _completionKeys(completionKeys)
{
	_recvBufs = new char*[maxClient];
	for (int i = 0; i < maxClient; ++i)
		_recvBufs[i] = new char[Receiver::RECV_BUF_SIZE];
	_recvs = new OverlappedEx[maxClient];
	for (int i = 0; i < maxClient; ++i)
	{
		memset(&_recvs[i].o, 0, sizeof(OVERLAPPED));
		// recvs->wsa.buf 라고 해서 안됐었음...
		_recvs[i].wsa.len = Receiver::RECV_BUF_SIZE;
		_recvs[i].wsa.buf = _recvBufs[i];
		_recvs[i].mode = IOMode::RECV;
		_recvs[i].sessionIdx = i;
	}
}
char* Receiver::getBuf(int idx) { return _recvBufs[idx]; }
void Receiver::onAccept(int idx) { this->pendingRecv(idx); }
void Receiver::onRecv(int idx) { this->pendingRecv(idx); }
#pragma endregion

#pragma region private
void Receiver::pendingRecv(int idx)
{
	CompletionKey& ck = _completionKeys[idx];
	DWORD bytes, flag = 0;

	WSARecv(ck.sock, &_recvs[idx].wsa, 1, &bytes, &flag, &_recvs[idx].o, NULL);
}
#pragma endregion
