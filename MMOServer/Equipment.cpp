#include "Equipment.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Player.h"

using json = nlohmann::json;
#pragma region public static
void Equipment::init()
{
	std::cout << "Equipment init...\n";
	std::ifstream fstream("C:/Users/taejeong/source/repos/ServerFramework/MMOServer/data/EquipmentData.json");
	json j = json::parse(fstream);
	j.get_to(s_equipmentData);
}
Equipment* Equipment::makeEquipment(const std::string& name)
{
	for (S_EquipmentData data : s_equipmentData)
		if (data.equipmentName == name)
		{
			Equipment* equipment = new Equipment(data);
			// TODO : DB에 추가
			return equipment;
		}
	std::cout << "equipment (" << name << ") doesnt exist. but you try to get data\n";
	return nullptr;
}
void Equipment::deleteEquipment(Equipment* equipment)
{
	// TODO : DB에서 삭제
	delete equipment;
}
#pragma endregion

#pragma region private static
#pragma endregion

#pragma region public
void Equipment::setNotify(protocol::mmo::GetEquipment_Notify& notify)
{
	notify.set_id(_data->templateID);

	notify.set_maxhp(_deltaStats.maxHp);
	notify.set_maxmp(_deltaStats.maxMp);
	notify.set_atk(_deltaStats.atk);
	notify.set_def(_deltaStats.def);
	notify.set_speed(_deltaStats.speed);
	notify.set_hp(_deltaStats.hp);
	notify.set_mp(_deltaStats.mp);
}
void Equipment::wear(Player* player)
{
	player->changeStats(_deltaStats, nullptr);
}
void Equipment::unwear(Player* player)
{
	player->changeStats(_deltaStats * -1, nullptr);
}
#pragma endregion

#pragma region private
#pragma endregion


std::vector<S_EquipmentData> Equipment::s_equipmentData;