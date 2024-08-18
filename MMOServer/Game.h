#pragma once
#include <vector>
#include <queue>
#include "DAT.h"
#include "E_GameObjectEvent.h"
class GameObject;
class Component;
class Shape;
class AABB;
class Area;
class Game
{
	friend class Shape; // add, remove, move에 접근하기 위해서
public:
	static const int UPDATE_DELTA_TIME = 100;

	static void init();
	static GameObject* instantiate(bool active = true, GameObject* copy = nullptr, GameObject* parent = nullptr);
	static void destroy(GameObject* obj);
	static void destroy(Component* component);
private:
	static void update();
	static void destroy();

	static void updateCollisions();
	static void invokeAreaEvent(const E_GameObjectEvent& evt, const std::pair<Area*, Area*>& collision);
	static int add(void* userData, const AABB& aabb);
	static void remove(int id);
	static void move(int id, const AABB& newAABB);
	
	static DAT s_dat;
	static std::vector<int> s_queryIds;
	static std::vector<std::pair<int, int>> s_candidates;
	static std::vector<std::pair<Area*, Area*>> s_collisions;

	static std::vector<GameObject*> s_gameObjects;
	static std::queue<GameObject*> s_destroyedGameObjects;
	static std::queue<Component*> s_destroyedComponents;
};


/*
* UPDATE_DELTA_TIME에서 broadphase가 시작되는 그 시점에 Area가 겹쳐지는지 판정을 하기 때문에
* 오브젝트가 이동하거나 active상태가 바뀌거나,
* Area가 broadphase에 추가되고 삭제되고 옮겨지거나 disable은 바로바로돼도 상관없음.
* 하지만 이벤트함수의 인자로 Area를 넘겨주기 때문에 실제로 메모리상에서 아예 삭제되는 건
* 바로 삭제하지 않고 가지고 있다가 이벤트함수들을 모두 호출해준 다음에 삭제가 이루어짐.
* 그래서 Area가 메모리에서 사라지는 경우(Area를 가지고 있는 게임 오브젝트를 삭제하려고 할때, Area 컴포넌트를 직접적으로 삭제하려고 할때)에
* 해당 값을 가지고 있는다.
*/