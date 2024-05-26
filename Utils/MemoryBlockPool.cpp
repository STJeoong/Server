#include "pch.h"
#include "MemoryBlockPool.h"

#pragma region public
void MemoryBlockPool::makePool(int blockSize)
{
	MemoryBlockPool* pool = new MemoryBlockPool(blockSize, MemoryBlockPool::DEFAULT_POOL_SIZE);
	s_vec.push_back({ blockSize, pool });
}
char* MemoryBlockPool::get(int blockSize)
{
	std::lock_guard<std::mutex> lock(s_mutex);
	MemoryBlockPool* pool = MemoryBlockPool::find(blockSize);
	return pool->get();
}
void MemoryBlockPool::release(int blockSize, char*& obj)
{
	std::lock_guard<std::mutex> lock(s_mutex);
	MemoryBlockPool* pool = MemoryBlockPool::find(blockSize);
	pool->release(obj);
}
#pragma endregion

#pragma region private
MemoryBlockPool* MemoryBlockPool::find(int blockSize)
{
	for (int i = 0; i < s_vec.size(); ++i)
		if (s_vec[i].first == blockSize)
			return s_vec[i].second;
	return nullptr;
}

MemoryBlockPool::MemoryBlockPool(int blockSize, int amount) : _blockSize(blockSize), _amount(amount), _currentAmount(amount)
{
	_pool = new char* [amount];
	for (int i = 0; i < amount; ++i)
		_pool[i] = new char[static_cast<int>(blockSize)];
}
MemoryBlockPool::~MemoryBlockPool()
{
	for (int i = 0; i < _amount; ++i)
		delete _pool[i];
	delete[] _pool;
}
char* MemoryBlockPool::get()
{
	if (_currentAmount == 0)
	{
		char* ret = new char[static_cast<int>(_blockSize)];
		return ret;
	}
	char* ret = _pool[_allocateIdx];
	_pool[_allocateIdx] = nullptr;
	_allocateIdx = (_allocateIdx + 1) % _amount;
	--_currentAmount;
	return ret;
}
void MemoryBlockPool::release(char*& obj)
{
	if (_currentAmount == _amount)
	{
		delete obj;
		obj = nullptr;
		return;
	}
	_pool[_releaseIdx] = obj;
	obj = nullptr;
	_releaseIdx = (_releaseIdx + 1) % _amount;
	++_currentAmount;
}
#pragma endregion

std::vector<std::pair<int, MemoryBlockPool*>> MemoryBlockPool::s_vec;
std::mutex MemoryBlockPool::s_mutex;