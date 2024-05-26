#include "pch.h"
#include "ObjectPool.h"

std::pair<size_t, ObjectPool*>* ObjectPool::s_pools = nullptr;
int ObjectPool::s_idx = 0;