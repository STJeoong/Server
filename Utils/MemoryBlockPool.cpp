#include "pch.h"
#include "MemoryBlockPool.h"

#pragma region public
void MemoryBlockPool::makePool(int blockSize)
{
	MemoryBlockPool* pool = new MemoryBlockPool(blockSize, MemoryBlockPool::DEFAULT_POOL_SIZE);
	s_vec.push_back({ blockSize, pool });
	//s_mp[blockSize] = new MemoryBlockPool(blockSize, MemoryBlockPool::DEFAULT_POOL_SIZE);
}
char* MemoryBlockPool::get(int blockSize)
{
	std::lock_guard<std::mutex> lock(s_mutex);
	MemoryBlockPool* pool = MemoryBlockPool::find(blockSize);
	return pool->get();
	//return s_mp[blockSize]->get();
}
void MemoryBlockPool::release(int blockSize, char*& obj)
{
	std::lock_guard<std::mutex> lock(s_mutex);
	MemoryBlockPool* pool = MemoryBlockPool::find(blockSize);
	pool->release(obj);
	//s_mp[blockSize]->release(obj);
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
	/*++_allocateIdx;
	if (_allocateIdx == _amount)
		_allocateIdx = 0;*/
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
	++_releaseIdx;
	if (_releaseIdx == _amount)
		_releaseIdx = 0;
	++_currentAmount;
}
#pragma endregion

std::vector<std::pair<int, MemoryBlockPool*>> MemoryBlockPool::s_vec;
std::unordered_map<int, MemoryBlockPool*> MemoryBlockPool::s_mp;
std::mutex MemoryBlockPool::s_mutex;