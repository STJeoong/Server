#pragma once
#include <vector>
#include "S_Equipment.h"
class Equipment
{
public:
	static void init();
	static S_Equipment getData(int templateID) { return s_equipmentData[templateID]; }
	static S_Equipment getData(const std::string& name);
private:
	static std::vector<S_Equipment> s_equipmentData;
};