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
	friend class Shape; // add, remove, move�� �����ϱ� ���ؼ�
	friend class GameObject; // �� �̵��Ҷ�
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
	std::vector<GameObject*> _gameObjects; // �ʿ� �����ϴ� ��� ���ӿ�����Ʈ��
	
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
* UPDATE_DELTA_TIME���� broadphase�� ���۵Ǵ� �� ������ Area�� ���������� ������ �ϱ� ������
* ������Ʈ�� �̵��ϰų� active���°� �ٲ�ų�,
* Area�� broadphase�� �߰��ǰ� �����ǰ� �Ű����ų� disable�� �ٷιٷεŵ� �������.
* ������ �̺�Ʈ�Լ��� ���ڷ� Area�� �Ѱ��ֱ� ������ ������ �޸𸮻󿡼� �ƿ� �����Ǵ� ��
* �ٷ� �������� �ʰ� ������ �ִٰ� �̺�Ʈ�Լ����� ��� ȣ������ ������ ������ �̷����.
* �׷��� Area�� �޸𸮿��� ������� ���(Area�� ������ �ִ� ���� ������Ʈ�� �����Ϸ��� �Ҷ�, Area ������Ʈ�� ���������� �����Ϸ��� �Ҷ�)��
* �ش� ���� ������ �ִ´�.
*/