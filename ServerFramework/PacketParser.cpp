#include "pch.h"
#include "PacketParser.h"
#include "S_PacketHeader.h"
#include "MemoryBlockPool.h"

#pragma region public
PacketParser::PacketParser(int maxClient)
{
	_readIdx = new UINT16[maxClient]{};
	_writeIdx = new UINT16[maxClient]{};
	_buf = new char*[maxClient];
	for (int i = 0; i < maxClient; ++i)
		_buf[i] = new char[maxClient]{};
}
void PacketParser::pushData(int serial, char* data, int len)
{
	int extraSpace = BUF_SIZE - _writeIdx[serial];
	if (extraSpace < len)
	{
		memcpy(_buf[serial], &_buf[serial][_readIdx[serial]], _writeIdx[serial] - _readIdx[serial]);
		_writeIdx[serial] -= _readIdx[serial];
		_readIdx[serial] = 0;
	}
	// 만약 버퍼 다 찼으면?
	memcpy(&_buf[serial][_writeIdx[serial]], data, len);
	_writeIdx[serial] += len;
}
void PacketParser::resetBuf(int idx)
{
	_readIdx[idx] = 0;
	_writeIdx[idx] = 0;
}
void PacketParser::collectData(int idx, S_EngineEvent& evt)
{
	S_PacketHeader* header = nullptr;
	
	int occupiedSpace = _writeIdx[idx] - _readIdx[idx];
	header = (S_PacketHeader*)&_buf[idx][_readIdx[idx]];
	if (occupiedSpace < sizeof(S_PacketHeader) || occupiedSpace < header->len)
		return;

	if (header->len <= 256)
	{
		evt.blockSize = Size::_256;
		evt.data = MemoryBlockPool::get((int)Size::_256);
	}
	else
	{
		evt.blockSize = Size::_1024;
		evt.data = MemoryBlockPool::get((int)Size::_1024);
	}
	memcpy(evt.data, &_buf[idx][_readIdx[idx]], header->len);

	_readIdx[idx] += header->len;
	if (_readIdx[idx] == _writeIdx[idx])
		_readIdx[idx] = _writeIdx[idx] = 0;
}
#pragma endregion

#pragma region private
#pragma endregion