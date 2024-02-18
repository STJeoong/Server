#include "pch.h"
#include "MemoryBlockPoolManager.h"
#include "MemoryBlockPool.h"
#pragma region public
char* MemoryBlockPoolManager::get(Size blockSize)
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_mp.find(blockSize) == _mp.end())
		_mp[blockSize] = new MemoryBlockPool(blockSize, MemoryBlockPoolManager::DEFAULT_POOL_SIZE);
	return _mp[blockSize]->get();
}
void MemoryBlockPoolManager::release(Size blockSize, char*& obj)
{
	std::lock_guard<std::mutex> lock(_mutex);
	// TODO : 사실 release할때는 find안해도 될듯
	if (_mp.find(blockSize) == _mp.end())
		_mp[blockSize] = new MemoryBlockPool(blockSize, MemoryBlockPoolManager::DEFAULT_POOL_SIZE);
	_mp[blockSize]->release(obj);
}
#pragma endregion

#pragma region private
MemoryBlockPoolManager::~MemoryBlockPoolManager()
{
	std::lock_guard<std::mutex> lock(_mutex);
	for (auto val : _mp)
		delete val.second;
}
#pragma endregion