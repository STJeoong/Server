#pragma once
#include <mutex>
#include "Define.h"
class MemoryBlockPool
{
public:
	MemoryBlockPool(Size blockSize, int amount);
	~MemoryBlockPool();
	char* get();
	void release(char*& obj);
private:


	Size _blockSize;
	char** _pool;

	int _allocateIdx = 0;
	int _releaseIdx = 0;
	int _amount;
	int _currentAmount;
};

