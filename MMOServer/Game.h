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
	friend class Shape; // add, remove, move�� �����ϱ� ���ؼ�
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
* UPDATE_DELTA_TIME���� broadphase�� ���۵Ǵ� �� ������ Area�� ���������� ������ �ϱ� ������
* ������Ʈ�� �̵��ϰų� active���°� �ٲ�ų�,
* Area�� broadphase�� �߰��ǰ� �����ǰ� �Ű����ų� disable�� �ٷιٷεŵ� �������.
* ������ �̺�Ʈ�Լ��� ���ڷ� Area�� �Ѱ��ֱ� ������ ������ �޸𸮻󿡼� �ƿ� �����Ǵ� ��
* �ٷ� �������� �ʰ� ������ �ִٰ� �̺�Ʈ�Լ����� ��� ȣ������ ������ ������ �̷����.
* �׷��� Area�� �޸𸮿��� ������� ���(Area�� ������ �ִ� ���� ������Ʈ�� �����Ϸ��� �Ҷ�, Area ������Ʈ�� ���������� �����Ϸ��� �Ҷ�)��
* �ش� ���� ������ �ִ´�.
*/