#include "Consume.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#pragma region public static
void Consume::init()
{
	std::cout << "Consume init...\n";
	std::ifstream fstream("C:/Users/taejeong/source/repos/ServerFramework/MMOServer/data/ConsumeData.json");
	json j = json::parse(fstream);
	j.get_to(s_consumeData);
}
S_ConsumeData Consume::getData(const std::string& name)
{
	for (S_ConsumeData data : s_consumeData)
		if (data.consumeName == name)
			return data;
	std::cout << "Consume (" << name << ") doesnt exist. but you try to get data\n";
	return {};
}
#pragma endregion

#pragma region private static
#pragma endregion

std::vector<S_ConsumeData> Consume::s_consumeData;