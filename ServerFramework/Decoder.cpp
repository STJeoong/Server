#include "pch.h"
#include "Decoder.h"
#include "I_EngineEventContainer.h"
#include "PacketParser.h"
#include "MemoryBlockPool.h"

#pragma region public
Decoder::Decoder(int engineID, I_EngineEventContainer* evtContainer, int maxClient) : _engineID(engineID), _evtContainer(evtContainer)
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
#pragma endregion

#pragma region private
void Decoder::threadMain()
{
	int serial;
	char* data;
	int len;
	Size blockSize;
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

		_parser->pushData(serial, data, len);
		MemoryBlockPool::release((int)blockSize, data);
		while (true)
		{
			S_EngineEvent evt = {};
			evt.serial = serial;
			evt.type = E_EngineEventType::EVENT_NET_RECV;
			_parser->collectData(serial, evt);
			// TODO : 복호화, 압축 해제
			if (evt.data == nullptr)
				break;
			_evtContainer->enqueue(_engineID, evt);
		}
	}
}
#pragma endregion