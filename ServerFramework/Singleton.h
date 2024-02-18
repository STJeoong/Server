#pragma once

// ���
// 1. ������, �Ҹ��ڸ� private
// 2. Singleton�� friend class��
template<typename T>
class Singleton
{
public:
	static T& getInstance() { static T instance; return instance; }
	Singleton(const Singleton& obj) = delete;
	Singleton(Singleton&& obj) = delete;
	Singleton& operator=(const Singleton& obj) = delete;
	Singleton& operator=(Singleton&& obj) = delete;
protected:
	Singleton() = default;
	virtual ~Singleton() = default;
};