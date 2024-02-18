#include "pch.h"
#include "Encoder.h"
#include "NetworkLayer.h"
#include "S_PacketHeader.h"
#pragma region public
Encoder::Encoder(NetworkLayer* network, int threadCount) : _network(network)
{
	for (int i = 0; i < threadCount; ++i)
		_threads.push_back(std::thread(&Encoder::threadMain, this));
}
Encoder::~Encoder()
{
	_stopThreads = true;
	_cv.notify_all();
	for (int i = 0; i < _threads.size(); ++i)
		_threads[i].join();
}
void Encoder::enqueue(S_RawData data)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push(data);
	_cv.notify_one();
}
#pragma endregion

#pragma region private
void Encoder::threadMain()
{
	S_RawData data;
	S_PacketHeader* header;
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_cv.wait(lock, [&] { return !_queue.empty() || _stopThreads; });
			if (_stopThreads)
				return;
			data = _queue.front();
			_queue.pop();
		}
		header = reinterpret_cast<S_PacketHeader*>(data.data);
		//TODO : 압축, 암호화 추가 ( 과정을 거치면서 패킷헤더의 _len값 변경해야됨 )
		_network->send(data.to, data.blockSize, header->len, data.data);
	}
}
#pragma endregion