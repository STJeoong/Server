#include "pch.h"
#include "Decoder.h"
#include "LogicLayer.h"
#include "PacketParser.h"
#include "MemoryBlockPoolManager.h"

#pragma region public
Decoder::Decoder(LogicLayer* logic, int threadCount) : _logic(logic)
{
	_parser = new PacketParser;
	for (int i = 0; i < threadCount; ++i)
		_threads.push_back(std::thread(&Decoder::threadMain, this));
}
Decoder::~Decoder()
{
	delete _parser;
	_stopThreads = true;
	_cv.notify_all();
	for (int i = 0; i < _threads.size(); ++i)
		_threads[i].join();
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
	S_LogicLayerArgs args;
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_cv.wait(lock, [&] { return !_queue.empty() || _stopThreads; });
			if (_stopThreads)
				return;
			std::tie(serial, data, len, blockSize) = _queue.front();
			_queue.pop();
		}
		args = {};
		args.serial = serial;
		args.type = E_EngineEvent::EVENT_NET_RECV;

		_parser->pushData(serial, data, len, args);
		MemoryBlockPoolManager::getInstance().release(blockSize, data);
		if (args.data == nullptr)
			continue;
		_logic->enqueue(args);
	}
}
#pragma endregion