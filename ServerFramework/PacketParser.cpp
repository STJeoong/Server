#include "pch.h"
#include "PacketParser.h"
#include "S_PacketHeader.h"
#include "MemoryBlockPoolManager.h"

#pragma region public
PacketParser::PacketParser(int maxClient)
{
	_readWriteMutex = new std::mutex[maxClient];
	_readIdx = new UINT16[maxClient]{};
	_writeIdx = new UINT16[maxClient]{};
	_buf = new char*[maxClient];
	for (int i = 0; i < maxClient; ++i)
		_buf[i] = new char[maxClient]{};
}
void PacketParser::pushData(int serial, char* data, int len, S_EngineEvent& evt)
{
	std::lock_guard<std::mutex> lock(_readWriteMutex[serial]);
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
	this->collectData(serial, evt);
	// TODO : 압축 해제, 복호화
}
void PacketParser::resetBuf(int idx)
{
	std::lock_guard<std::mutex> lock(_readWriteMutex[idx]);
	_readIdx[idx] = 0;
	_writeIdx[idx] = 0;
}
#pragma endregion

#pragma region private
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
		evt.data = MemoryBlockPoolManager::getInstance().get(Size::_256);
	}
	else
	{
		evt.blockSize = Size::_1024;
		evt.data = MemoryBlockPoolManager::getInstance().get(Size::_1024);
	}
	memcpy(evt.data, &_buf[idx][_readIdx[idx]], header->len);

	_readIdx[idx] += header->len;
	if (_readIdx[idx] == _writeIdx[idx])
		_readIdx[idx] = _writeIdx[idx] = 0;
}
#pragma endregion