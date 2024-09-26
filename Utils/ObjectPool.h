#pragma once
#include <unordered_map>
#include <functional>
#include <typeinfo>
#include <vector>
class ObjectPool
{
public:
	static const int MAX_POOL_SIZE = 1000;
	template<typename T>
	static void makePool(size_t size, std::function<void*()> createFunc, std::function<void(void*)> actionOnGet, 
																	std::function<void(void*)> actionOnRelease);
	// if you try to get object from not initialized pool, your program is going to be crashed.
	template<typename T>
	static T* get();
	// if you try to release object to not initialized pool, your program is going to be crashed.
	template<typename T>
	static void release(T* obj);
	template<typename T>
	static void useActionOnGet(bool flag);
	template<typename T>
	static void useActionOnRelease(bool flag);
private:
	template<typename T>
	static ObjectPool* findPool();

	static std::pair<size_t, ObjectPool*>* s_pools;
	static int s_idx;

	ObjectPool(size_t size, std::function<void*()> createFunc, std::function<void(void*)> actionOnGet,
															std::function<void(void*)> actionOnRelease);
	~ObjectPool() = default;

	void** _objs;
	size_t _size;
	size_t _allocateIdx = 0;
	size_t _releaseIdx = 0;
	size_t _currentSize = 0;
	std::function<void*()> _createFunc;
	std::function<void(void*)> _actionOnGet;
	std::function<void(void*)> _actionOnRelease;
	bool _useActionOnGet = true;
	bool _useActionOnRelease = true;
};
template<typename T>
inline void ObjectPool::makePool(size_t size, std::function<void*()> createFunc, std::function<void(void*)> actionOnGet,
																				std::function<void(void*)> actionOnRelease)
{
	if (s_pools == nullptr)
		s_pools = new std::pair<size_t, ObjectPool*>[ObjectPool::MAX_POOL_SIZE];
	const std::type_info& info = typeid(T);
	size_t typeId = info.hash_code();

	ObjectPool* pool = new ObjectPool(size, createFunc, actionOnGet, actionOnRelease);
	s_pools[s_idx++] = { typeId, pool };
}
template<typename T>
inline T* ObjectPool::get()
{
	ObjectPool* pool = ObjectPool::findPool<T>();
	T* ret = nullptr;

	if (pool->_currentSize == 0)
		ret = reinterpret_cast<T*>(pool->_createFunc());
	else
	{
		//ret = reinterpret_cast<T*>(pool->getObj());
		ret = reinterpret_cast<T*>(pool->_objs[pool->_allocateIdx]);
		pool->_objs[pool->_allocateIdx] = nullptr;
		++pool->_allocateIdx;
		if (pool->_allocateIdx == pool->_size)
			pool->_allocateIdx = 0;
		--pool->_currentSize;
	}
	if (pool->_useActionOnGet)
		pool->_actionOnGet(ret);
	return ret;
}
template<typename T>
inline void ObjectPool::release(T* obj)
{
	ObjectPool* pool = ObjectPool::findPool<T>();

	if (pool->_currentSize == pool->_size)
		delete obj;
	else
	{
		//pool->releaseObj(obj);
		pool->_objs[pool->_releaseIdx] = obj;
		++pool->_releaseIdx;
		if (pool->_releaseIdx == pool->_size)
			pool->_releaseIdx = 0;
		++pool->_currentSize;
	}
	if (pool->_useActionOnRelease)
		pool->_actionOnRelease(obj);
}

template<typename T>
inline void ObjectPool::useActionOnGet(bool flag)
{
	ObjectPool* pool = ObjectPool::findPool<T>();
	pool->_useActionOnGet = flag;
}
template<typename T>
inline void ObjectPool::useActionOnRelease(bool flag)
{
	ObjectPool* pool = ObjectPool::findPool<T>();
	pool->_useActionOnRelease = flag;
}
template<typename T>
inline ObjectPool* ObjectPool::findPool()
{
	const std::type_info& info = typeid(T);
	size_t typeId = info.hash_code();
	for (int i = 0; i < s_idx; ++i)
		if (s_pools[i].first == typeId)
			return s_pools[i].second;
	return nullptr;
}
inline ObjectPool::ObjectPool(size_t size, std::function<void* ()> createFunc, std::function<void(void*)> actionOnGet,
	std::function<void(void*)> actionOnRelease)
	: _size(size), _createFunc(createFunc), _actionOnGet(actionOnGet), _actionOnRelease(actionOnRelease)
{
	_objs = new void*[size];
	_currentSize = size;
	for (int i = 0; i < size; ++i)
		_objs[i] = createFunc();
}