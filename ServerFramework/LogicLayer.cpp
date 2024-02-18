#include "pch.h"
#include "LogicLayer.h"
#include "I_Server.h"
#include "MemoryBlockPoolManager.h"

#pragma region public
LogicLayer::~LogicLayer()
{
	stopThread = true;
	_cv.notify_one();
	_thread.join();
}
void LogicLayer::enqueue(S_LogicLayerArgs args)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push(args);
	_cv.notify_one();
}
#pragma endregion

#pragma region private
void LogicLayer::threadMain()
{
	S_LogicLayerArgs args;
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_cv.wait(lock, [&] { return !_queue.empty() || stopThread; });
			if (stopThread)
				return;
			args = _queue.front();
			_queue.pop();
		}
		_server->process(args.serial, args.type, args.data);
		if (args.data != nullptr)
			MemoryBlockPoolManager::getInstance().release(args.blockSize, args.data);
	}
}
#pragma endregion