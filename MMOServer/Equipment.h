#pragma once
#include <vector>
#include "MMO_protocol.pb.h"
#include "S_EquipmentData.h"
#include "S_Stats.h"
class Player;
class Equipment
{
public:
	static void init();
	static Equipment* makeEquipment(const std::string& name);
	static void deleteEquipment(Equipment* equipment);
private:
	static Equipment* load() {} // TODO
	static void unload(Equipment* equipment) { delete equipment; }

	static std::vector<S_EquipmentData> s_equipmentData;


public:
	void setNotify(protocol::mmo::GetEquipment_Notify& notify);
	protocol::mmo::E_Equipment type() const { return _data->type; }
	void wear(Player* player);
	void unwear(Player* player);
private:
	Equipment(const S_EquipmentData& data) : _data(&data), _deltaStats(data.deltaStats) {}
	Equipment(const Equipment& other) = delete;
	~Equipment() = default;


	const S_EquipmentData* _data = nullptr;
	S_Stats _deltaStats = {};
};