#pragma once
#include <unordered_map>
#include <mutex>
#include "Singleton.h"
#include "Define.h"

class MemoryBlockPool;
class MemoryBlockPoolManager : public Singleton<MemoryBlockPoolManager>
{
	friend class Singleton;

	static const UINT32 DEFAULT_POOL_SIZE = 1000;
public:
	char* get(Size blockSize);
	void release(Size blockSize, char*& obj);
private:
	MemoryBlockPoolManager() = default;
	~MemoryBlockPoolManager();


	std::unordered_map<Size, MemoryBlockPool*> _mp;
	std::mutex _mutex;
};

