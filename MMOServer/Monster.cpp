#include "Monster.h"
#include "GameObject.h"
#include "Area.h"
#include "Player.h"
#include "S_MonsterData.h"
#include <vector>
#include <fstream>


#include "Map.h"
using json = nlohmann::json;
#pragma region public static
void Monster::init()
{
	// TODO : 설정파일로 몬스터 정보 가져와서 한번에 소환하기
	/*std::ifstream fstream("C:/Users/taejeong/source/repos/ServerFramework/MMOServer/data/MonsterData.json");
	std::vector<S_MonsterData> data;
	json j = json::parse(fstream);
	j.get_to(data);

	for (int i = 0; i < data.size(); ++i);*/
}
#pragma endregion

#pragma region private static
#pragma endregion




#pragma region public
#pragma endregion

#pragma region protected
#pragma endregion

#pragma region private
#pragma endregion