#include "pch.h"
#include "Decoder.h"
#include "EngineEventContainer.h"
#include "PacketParser.h"
#include "MemoryBlockPoolManager.h"

#pragma region public
Decoder::Decoder(int engineID, EngineEventContainer* evtContainer, int maxClient) : _engineID(engineID), _evtContainer(evtContainer)
{
	_parser = new PacketParser(maxClient);
	_thread = std::thread(&Decoder::threadMain, this);
}
Decoder::~Decoder()
{
	delete _parser;
	_stopThread = true;
	_cv.notify_one();
	_thread.join();
}
void Decoder::enqueue(int serial, char* data, int len, Size blockSize)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push({ serial, data, len, blockSize });
	_cv.notify_one();
}
void Decoder::reset(int serial) { _parser->resetBuf(serial); }
#pragma endregion

#pragma region private
void Decoder::threadMain()
{
	int serial;
	char* data;
	int len;
	Size blockSize;
	S_EngineEvent evt;
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_cv.wait(lock, [&] { return !_queue.empty() || _stopThread; });
			if (_stopThread)
				return;
			std::tie(serial, data, len, blockSize) = _queue.front();
			_queue.pop();
		}
		evt = {};
		evt.serial = serial;
		evt.type = E_EngineEventType::EVENT_NET_RECV;

		_parser->pushData(serial, data, len);
		MemoryBlockPoolManager::getInstance().release(blockSize, data);
		while (true)
		{
			_parser->collectData(serial, evt);
			// TODO : 복호화, 압축 해제
			if (evt.data == nullptr)
				break;
			_evtContainer->enqueue(_engineID, evt);
		}
	}
}
#pragma endregion