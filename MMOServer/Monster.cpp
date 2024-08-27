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

using namespace protocol::mmo;
using json = nlohmann::json;
#pragma region public static
void Monster::init()
{
	MMOServerBroadcaster::onMonsterRespawn += Monster::respawn;

	// TODO : 설정파일로 몬스터 정보 가져와서 한번에 소환하기
	std::ifstream fstream("C:/Users/taejeong/source/repos/ServerFramework/MMOServer/data/MonsterData.json");
	std::vector<S_MonsterData> data;
	json j = json::parse(fstream);
	j.get_to(data);

	for (int i = 0; i < data.size(); ++i)
		Monster::spawn(data[i]);
}
#pragma endregion

#pragma region private static
void Monster::spawn(const S_MonsterData& data)
{
	for (int i = 0; i < data.spawnInfo.size(); ++i)
	{
		auto [mapName, howMany] = data.spawnInfo[i];
		Map* map = Map::getMap(mapName);
		for (int j = 0; j < howMany; ++j)
		{
			Monster* monster = map->instantiate<Monster>(true);
			monster->state(E_ObjectState::IDLE);
			monster->_objArea = monster->addComponent<Area>();
			monster->_objArea->layer(E_Layer::MONSTER_OBJ);
			MonsterController* controller = monster->addComponent<MonsterController>();

			monster->_objArea->addShape(new Rectangular(monster->_objArea, -data.extraCellSize, -data.extraCellSize, 2 * data.extraCellSize + 1, 2 * data.extraCellSize + 1));

			if (data.isAggressive)
			{
				Area* aggressiveArea = monster->addComponent<Area>();
				aggressiveArea->layer(E_Layer::MONSTER_AGGRESSIVE);
				aggressiveArea->addShape(new Rectangular(aggressiveArea, data.offsetY, data.offsetX, data.yExtension, data.xExtension));
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
			monster->_initTF.set_dir(E_Dir::BOTTOM);
			monster->_initTF.set_y(y);
			monster->_initTF.set_x(x);
			monster->transform(y, x, E_Dir::BOTTOM);
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
		stats.hp = stats.maxHp;
		stats.mp = stats.maxMp;
		stats.attack = stats.defaultAttack;
		stats.defence = stats.defaultDefence;
		stats.speed = stats.defaultSpeed;


		// 리스폰
		monster->active(true);
	}
}
#pragma endregion




#pragma region public
void Monster::broadcast(protocol::mmo::E_PacketID packetID)
{
	for (Area* other : _objArea->overlappedAreas())
		if (other->layer() == E_Layer::AOI)
			Utils::send(Utils::getID(other->gameObject()->id()), packetID, 0);
}
void Monster::broadcast(E_PacketID packetID, google::protobuf::Message& message)
{
	for (Area* other : _objArea->overlappedAreas())
		if (other->layer() == E_Layer::AOI)
			Utils::send(Utils::getID(other->gameObject()->id()), packetID, 0, message);
}
void Monster::stats(const S_Stats& val)
{
	_stats = val;
	if (_stats.hp <= 0)
		Monster::s_deadMonsters.push(this);
	this->active(false);
}
#pragma endregion

#pragma region protected
#pragma endregion

#pragma region private
#pragma endregion

std::queue<Monster*> Monster::s_deadMonsters;