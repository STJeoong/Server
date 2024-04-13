#pragma once
#include <unordered_map>
#include <mutex>
class MemoryBlockPool
{
public:
	static char* get(int blockSize);
	static void release(int blockSize, char*& obj);
private:
	MemoryBlockPool(int blockSize, int amount);
	~MemoryBlockPool();
	char* get();
	void release(char*& obj);

	static const int DEFAULT_POOL_SIZE = 1000;
	static std::unordered_map<int, MemoryBlockPool*> s_mp;
	static std::mutex s_mutex;

	int _blockSize;
	char** _pool;

	int _allocateIdx = 0;
	int _releaseIdx = 0;
	int _amount;
	int _currentAmount;
};

