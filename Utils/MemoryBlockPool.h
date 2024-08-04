#pragma once
#include <unordered_map>
#include <mutex>
class MemoryBlockPool
{
public:
	static const int MAX_POOL_SIZE = 20;
	static void makePool(int blockSize);
	static char* get(int blockSize);
	static void release(int blockSize, char*& obj);
private:
	static const int DEFAULT_POOL_SIZE = 1000;
	static MemoryBlockPool* find(int blockSize);

	MemoryBlockPool(int blockSize, int amount);
	~MemoryBlockPool();
	char* get();
	void release(char*& obj);

	static std::pair<int, MemoryBlockPool*>* s_pools;
	static int s_idx;

	std::mutex _mutex;
	int _blockSize;
	char** _pool;

	int _allocateIdx = 0;
	int _releaseIdx = 0;
	int _amount;
	int _currentAmount;
};

