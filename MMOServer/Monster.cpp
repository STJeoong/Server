#include "Monster.h"
#include "MMOServerBroadcaster.h"
#include "GameObject.h"
#include "Area.h"
#include "Player.h"
#include "S_MonsterData.h"
#include <vector>
#include <fstream>
#include "Map.h"
#include "Utils.h"
#include "Rectangular.h"
#include "MonsterController.h"
#include "S_EquipmentData.h"
#include "S_ConsumeData.h"
#include "Equipment.h"

using namespace protocol::mmo;
using json = nlohmann::json;
#pragma region public static
void Monster::init()
{
	MMOServerBroadcaster::onMonsterRespawn += Monster::respawn;

	std::cout << "Monster init...\n";
	// TODO : 설정파일로 몬스터 정보 가져와서 한번에 소환하기
	std::ifstream fstream("C:/Users/taejeong/source/repos/ServerFramework/MMOServer/data/MonsterData.json");
	json j = json::parse(fstream);
	j.get_to(s_monsterData);

	for (int i = 0; i < s_monsterData.size(); ++i)
		Monster::spawn(s_monsterData[i]);
}
#pragma endregion

#pragma region private static
void Monster::spawn(const S_MonsterData& data)
{
	for (int i = 0; i < data.spawnInfo.size(); ++i)
	{
		const std::string& mapName = data.spawnInfo[i].mapName;
		int howMany = data.spawnInfo[i].howMany;
		Map* map = Map::getMap(mapName);
		for (int j = 0; j < howMany; ++j)
		{
			// Area는 가장 마지막에 addComponent해줘야됨.
			Monster* monster = map->instantiate<Monster>(true);
			monster->_data = &data;
			monster->state(E_ObjectState::IDLE);
			monster->_objArea = monster->addComponent<Area>();
			monster->_objArea->layer(E_Layer::MONSTER_OBJ);
			MonsterController* controller = monster->addComponent<MonsterController>();

			{
				S_RectDefine rect = {};
				rect.offsetY = -data.extraCellSize;
				rect.offsetX = -data.extraCellSize;
				rect.yExtension = 2 * data.extraCellSize + 1;
				rect.xExtension = 2 * data.extraCellSize + 1;
				monster->_objArea->addShape(new Rectangular(monster->_objArea, rect));
			}

			if (data.isAggressive)
			{
				Area* aggressiveArea = monster->addComponent<Area>();
				controller->aggressiveArea(aggressiveArea);
				aggressiveArea->layer(E_Layer::MONSTER_AGGRESSIVE);
				aggressiveArea->addShape(new Rectangular(aggressiveArea, data.aggressiveArea));
			}

			{
				// 몬스터 기본공격
				Area* normalAttackArea = monster->addComponent<Area>();
				controller->normalAttackArea(normalAttackArea);
				normalAttackArea->layer(E_Layer::MONSTER_NORMAL_ATTACK);
				normalAttackArea->addShape(new Rectangular(normalAttackArea, data.normalAttack.areaDefine));
				normalAttackArea->fixedRotation(data.normalAttack.fixedRotation);
				normalAttackArea->enable(false);
			}

			// TODO : 몬스터 스탯
			monster->_stats = data.stats;
			monster->id(Utils::createID(E_ObjectType::MONSTER, data.templateID, j));
			int y, x;
			while (true)
			{
				y = Utils::randomVal(map->yMin(), map->yMax());
				x = Utils::randomVal(map->xMin(), map->xMax());
				if (map->canGo(y, x))
					break;
			}
			monster->_initTF.set_dir((E_Dir)0);
			monster->_initTF.set_y(y);
			monster->_initTF.set_x(x);
			monster->transform(y, x, (E_Dir)0);
			monster->flipX(false);
		}
	}
}
void Monster::respawn()
{
	while (!s_deadMonsters.empty())
	{
		Monster* monster = s_deadMonsters.front();
		s_deadMonsters.pop();
		const TransformInt& initTF = monster->_initTF;
		S_Stats& stats = monster->_stats;

		// 스탯 및 위치값 다시 초기화
		monster->transform(initTF.y(), initTF.x(), initTF.dir());
		monster->flipX(false);
		stats = (*(monster->_data)).stats;

		// 리스폰
		monster->active(true);
	}
}
#pragma endregion




#pragma region public
void Monster::broadcastPacket(protocol::mmo::E_PacketID packetID)
{
	for (Area* other : _objArea->overlappedAreas())
		if (other->layer() == E_Layer::AOI)
		{
			Player* player = reinterpret_cast<Player*>(other->gameObject());
			Utils::send(player->networkSerial(), packetID, 0);
		}
}
void Monster::broadcastPacket(E_PacketID packetID, google::protobuf::Message& message)
{
	for (Area* other : _objArea->overlappedAreas())
		if (other->layer() == E_Layer::AOI)
		{
			Player* player = reinterpret_cast<Player*>(other->gameObject());
			Utils::send(player->networkSerial(), packetID, 0, message);
		}
}
void Monster::addBuff(Buff* buff) { _buff.push_back(buff); }
void Monster::removeBuff(Buff* buff)
{
	auto it = std::find(_buff.begin(), _buff.end(), buff);
	if (it != _buff.end())
		_buff.erase(it);
}
void Monster::addCC(CC* cc) { _cc.push_back(cc); }
void Monster::removeCC(CC* cc)
{
	auto it = std::find(_cc.begin(), _cc.end(), cc);
	if (it != _cc.end())
		_cc.erase(it);
}
void Monster::addPersistentChangeStats(PersistentChangeStats* persistent) { _persistent.push_back(persistent); }
void Monster::removePersistentChangeStats(PersistentChangeStats* persistent)
{
	auto it = std::find(_persistent.begin(), _persistent.end(), persistent);
	if (it != _persistent.end())
		_persistent.erase(it);
}
void Monster::changeStats(S_Stats delta, GameObject* who)
{
	_stats += delta;
	if (_stats.hp <= 0)
	{
		s_deadMonsters.push(this);
		this->active(false);
		this->giveItems(reinterpret_cast<Player*>(who));
		return;
	}

	ChangeStats_Notify notify = {};

	notify.set_id(this->id());
	notify.set_maxhp(_stats.maxHp);
	notify.set_maxmp(_stats.maxMp);
	notify.set_atk(_stats.atk);
	notify.set_def(_stats.def);
	notify.set_speed(_stats.speed);
	notify.set_hp(_stats.hp);
	notify.set_mp(_stats.mp);

	this->broadcastPacket(E_PacketID::CHANGE_STATS_NOTIFY, notify);
}
void Monster::giveItems(Player* player)
{
	if (player == nullptr) return;

	for (auto data : _data->equipDropInfo)
	{
		if (!player->hasSpaceInEquipment())
			break;
		const std::string& name = data.first;
		int percent = data.second;
		if (Utils::gacha(percent))
			player->getEquipment(Equipment::makeEquipment(name));
	}
	// TODO : 물약
}
#pragma endregion

#pragma region protected
#pragma endregion

#pragma region private
#pragma endregion

std::queue<Monster*> Monster::s_deadMonsters;
std::vector<S_MonsterData> Monster::s_monsterData;