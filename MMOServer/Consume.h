#pragma once
#include <vector>
#include "S_Consume.h"
class Consume
{
public:
	static void init();
	static S_Consume getData(int templateID) { return s_consumeData[templateID]; }
	static S_Consume getData(const std::string& name);
private:
	static std::vector<S_Consume> s_consumeData;
};