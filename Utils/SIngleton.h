#pragma once

// 사용
// 1. 생성자, 소멸자를 private
// 2. Singleton을 friend class로
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