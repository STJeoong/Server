#include "pch.h"
#include "ObjectPool.h"

std::unordered_map<size_t, std::queue<void*>> ObjectPool::s_mp;
std::unordered_map<size_t, size_t> ObjectPool::s_size;
std::unordered_map<size_t, std::function<void* ()>> ObjectPool::s_createFunc;
std::unordered_map<size_t, std::function<void(void*)>> ObjectPool::s_actionOnGet;
std::unordered_map<size_t, std::function<void(void*)>> ObjectPool::s_actionOnRelease;