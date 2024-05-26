#pragma once
#include <unordered_map>
#include <vector>
#include <mutex>
class MemoryBlockPool
{
public:
	static void makePool(int blockSize);
	static char* get(int blockSize);
	static void release(int blockSize, char*& obj);
private:
	static MemoryBlockPool* find(int blockSize);

	MemoryBlockPool(int blockSize, int amount);
	~MemoryBlockPool();
	char* get();
	void release(char*& obj);

	static const int DEFAULT_POOL_SIZE = 1000;
	static std::vector<std::pair<int, MemoryBlockPool*>> s_vec;
	static std::unordered_map<int, MemoryBlockPool*> s_mp;
	static std::mutex s_mutex;

	int _blockSize;
	char** _pool;

	int _allocateIdx = 0;
	int _releaseIdx = 0;
	int _amount;
	int _currentAmount;
};

