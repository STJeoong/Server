#pragma once

class Utils
{
public:
	template<typename T>
	static T clamp(T v, T hi, T lo) { return v < lo ? lo : v > hi ? hi : v; }
};