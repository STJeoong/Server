#pragma once
#include <unordered_map>
#include <queue>
#include <functional>
#include <typeinfo>
class ObjectPool
{
public:
	template<typename T>
	static void makePool(size_t size, std::function<void*()> createFunc, std::function<void(void*)> actionOnGet,
						std::function<void(void*)> actionOnRelease);
	// if you try to get object from not initialized pool, your program is going to be crashed.
	template<typename T>
	static T* get();
	// if you try to release object to not initialized pool, your program is going to be crashed.
	template<typename T>
	static void release(T*& obj);
private:

	static std::unordered_map<size_t, std::queue<void*>> s_mp;
	static std::unordered_map<size_t, size_t> s_size;
	static std::unordered_map<size_t, std::function<void*()>> s_createFunc;
	static std::unordered_map<size_t, std::function<void(void*)>> s_actionOnGet;
	static std::unordered_map<size_t, std::function<void(void*)>> s_actionOnRelease;
};
template<typename T>
inline void ObjectPool::makePool(size_t size, std::function<void*()> createFunc, std::function<void(void*)> actionOnGet,
								std::function<void(void*)> actionOnRelease)
{
	const std::type_info& info = typeid(T);
	size_t typeId = info.hash_code();
	if (s_mp.find(typeId) != s_mp.end()) return;

	s_mp[typeId] = std::queue<void*>();
	s_size[typeId] = size;
	s_createFunc[typeId] = createFunc;
	s_actionOnGet[typeId] = actionOnGet;
	s_actionOnRelease[typeId] = actionOnRelease;
	for (int i = 0; i < size; ++i)
		s_mp[typeId].push(createFunc());
}
template<typename T>
inline T* ObjectPool::get()
{
	const std::type_info& info = typeid(T);
	size_t typeId = info.hash_code();
	std::queue<void*>& q = s_mp[typeId];
	T* ret = nullptr;

	if (q.empty()) ret = s_createFunc[typeId]();
	else
	{
		ret = q.front();
		q.pop();
	}
	s_actionOnGet[typeId](ret);
	return ret;
}
template<typename T>
inline void ObjectPool::release(T*& obj)
{
	const std::type_info& info = typeid(T);
	size_t typeId = info.hash_code();
	std::queue<void*>& q = s_mp[typeId];

	if (q.size() == s_size[typeId]) delete obj;
	else q.push(obj);
	s_actionOnRelease[typeId](obj);
	obj = nullptr;
}