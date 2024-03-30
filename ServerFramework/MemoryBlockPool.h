#pragma once
#include <unordered_map>
#include <mutex>
#include "Define.h"
class MemoryBlockPool
{
public:
	static char* get(Size blockSize);
	static void release(Size blockSize, char*& obj);
private:
	MemoryBlockPool(Size blockSize, int amount);
	~MemoryBlockPool();
	char* get();
	void release(char*& obj);

	static const int DEFAULT_POOL_SIZE = 1000;
	static std::unordered_map<Size, MemoryBlockPool*> s_mp;
	static std::mutex s_mutex;

	Size _blockSize;
	char** _pool;

	int _allocateIdx = 0;
	int _releaseIdx = 0;
	int _amount;
	int _currentAmount;
};

