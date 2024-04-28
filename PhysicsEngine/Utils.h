#pragma once

class Utils
{
public:
	template<typename T>
	static T clamp(const T& v, const T& hi, const T& lo) { return v < lo ? lo : v > hi ? hi : v; }
};