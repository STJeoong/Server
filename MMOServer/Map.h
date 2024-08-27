#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <optional>
#include <queue>
#include "E_GameObjectEvent.h"


namespace MapName
{
	const std::string MAIN = "Main";
}

class GameObject;
class Component;
class DAT;
class Area;
class AABB;
class Map
{
	friend class Shape; // add, remove, move에 접근하기 위해서
	friend class GameObject; // 맵 이동할때
public:
	static void load();
	static Map* getMap(const std::string& mapName);
private:
	static std::unordered_map<std::string, Map*> s_maps;



public:
	template<typename T>
	T* instantiate(bool isActive, T* copy = nullptr, T* parent = nullptr);
	void destroy(GameObject* obj);
	void destroy(Component* component);

	int xMin() const { return _xMin; }
	int xMax() const { return _xMax; }
	int yMin() const { return _yMin; }
	int yMax() const { return _yMax; }
	void overlapsBox(const AABB& box, int filter, std::vector<GameObject*>& list);
	std::pair<int, int> basePoint() const { return { _basePointY, _basePointX }; }
	bool canGo(int y, int x);
	std::optional<std::pair<int, int>> findPath(const std::pair<int, int>& start, const std::pair<int, int>& dest);
private:
	Map(const std::filesystem::path& path);
	~Map();
	void update();
	void updateCollisions();
	void invokeAreaEvent(const E_GameObjectEvent& evt, const std::pair<Area*, Area*>& collision);
	void destroy();

	int add(Area* userData, const AABB& aabb);
	void remove(int id);
	void move(int id, const AABB& newAABB);

	int _xMin;
	int _xMax;
	int _yMin;
	int _yMax;
	int _basePointY;
	int _basePointX;
	std::vector<std::vector<bool>> _canGo;
	std::vector<GameObject*> _gameObjects; // 맵에 존재하는 모든 게임오브젝트들
	
	DAT* _dat = nullptr;
	std::vector<int> _queryIDs;
	std::vector<std::pair<int, int>> _candidates;
	std::vector<std::pair<Area*, Area*>> _collisions;

	std::queue<GameObject*> _destroyedGameObjects;
	std::queue<Component*> _destroyedComponents;
};


// T must inherit GameObject class.
template<typename T>
inline T* Map::instantiate(bool isActive, T* copy, T* parent)
{
	T* ret = nullptr;
	if (copy != nullptr)
		ret = new T(isActive, copy, parent);
	else
		ret = new T(isActive, parent);
	_gameObjects.push_back(ret);
	ret->_map = this;
	return ret;
}

/*
* UPDATE_DELTA_TIME에서 broadphase가 시작되는 그 시점에 Area가 겹쳐지는지 판정을 하기 때문에
* 오브젝트가 이동하거나 active상태가 바뀌거나,
* Area가 broadphase에 추가되고 삭제되고 옮겨지거나 disable은 바로바로돼도 상관없음.
* 하지만 이벤트함수의 인자로 Area를 넘겨주기 때문에 실제로 메모리상에서 아예 삭제되는 건
* 바로 삭제하지 않고 가지고 있다가 이벤트함수들을 모두 호출해준 다음에 삭제가 이루어짐.
* 그래서 Area가 메모리에서 사라지는 경우(Area를 가지고 있는 게임 오브젝트를 삭제하려고 할때, Area 컴포넌트를 직접적으로 삭제하려고 할때)에
* 해당 값을 가지고 있는다.
*/