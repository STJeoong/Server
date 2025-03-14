#pragma once
#include <vector>
#include "S_ConsumeData.h"
class Consume
{
public:
	static void init();
	static S_ConsumeData getData(int templateID) { return s_consumeData[templateID]; }
	static S_ConsumeData getData(const std::string& name);
private:
	static std::vector<S_ConsumeData> s_consumeData;
};