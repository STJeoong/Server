#include "Equipment.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
#pragma region public static
void Equipment::init()
{
	std::cout << "Equipment init...\n";
	std::ifstream fstream("C:/Users/taejeong/source/repos/ServerFramework/MMOServer/data/EquipmentData.json");
	json j = json::parse(fstream);
	j.get_to(s_equipmentData);
}
S_Equipment Equipment::getData(const std::string& name)
{
	for (S_Equipment data : s_equipmentData)
		if (data.equipmentName == name)
			return data;
	std::cout << "equipment (" << name << ") doesnt exist. but you try to get data\n";
	return {};
}
#pragma endregion

#pragma region private static
#pragma endregion

std::vector<S_Equipment> Equipment::s_equipmentData;