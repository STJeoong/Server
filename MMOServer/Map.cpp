#include "Map.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <limits>
#include "Utils.h"
#include "MMOServerBroadcaster.h"
#include "GameObject.h"
#include "Component.h"
#include "DAT.h"
#include "LayerFilter.h"
#include "Area.h"
#include "AABB.h"

#ifdef max
#undef max
#endif

#pragma region public static
void Map::load()
{
	std::filesystem::path directoryPath = "C:/Users/taejeong/source/repos/ServerFramework/MMOServer/data/map";
	if (!std::filesystem::exists(directoryPath) || !std::filesystem::is_directory(directoryPath))
	{
		std::cout << directoryPath << " isn't directory\n";
		return;
	}
	std::cout << "map loading...\n";
	for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
	{
		std::string fileName = entry.path().filename().string();
		std::string mapName = fileName.substr(0, fileName.find('_')); // _Collision도 key값에서 뺐음.
		s_maps[mapName] = new Map(entry.path());
		//s_maps[Utils::eraseFileExtensionName(entry.path().filename().string())] = new Map(entry.path());
	}
}
Map* Map::getMap(const std::string& mapName) { return s_maps[mapName]; }
#pragma endregion

#pragma region private static
#pragma endregion





#pragma region public
void Map::destroy(GameObject* obj)
{
	_destroyedGameObjects.push(obj);
	obj->broadcastToComponents(E_GameObjectEvent::DESTROY, nullptr);
	_gameObjects.erase(std::find(_gameObjects.begin(), _gameObjects.end(), obj));

	if (obj->_parent != nullptr)
	{
		std::vector<GameObject*>& children = obj->_parent->_children;
		children.erase(std::find(children.begin(), children.end(), obj));
	}
	if (!obj->_children.empty())
	{
		static std::stack<GameObject*> stk; // for performance, this variable is static.
		for (GameObject* child : obj->_children)
			stk.push(child);
		while (!stk.empty())
		{
			GameObject* g = stk.top();
			stk.pop();
			for (GameObject* child : g->_children)
				stk.push(child);
			g->broadcastToComponents(E_GameObjectEvent::DESTROY, nullptr);
			_gameObjects.erase(std::find(_gameObjects.begin(), _gameObjects.end(), g));
			_destroyedGameObjects.push(g);
		}
	}
}
void Map::destroy(Component* component)
{
	component->invoke(E_GameObjectEvent::DESTROY, nullptr);
	std::vector<Component*>& components = component->gameObject()->_components;
	components.erase(std::find(components.begin(), components.end(), component));
	_destroyedComponents.push(component);
}
bool Map::canGo(int y, int x)
{
	if (y < _yMin || y > _yMax || x < _xMin || x > _xMax)
		return false;
	return _canGo[y - _yMin][x - _xMin];
}
std::optional<std::pair<int, int>> Map::findPath(const std::pair<int, int>& start, const std::pair<int, int>& dest)
{
	static std::priority_queue<std::tuple<int, int, int, int>, std::vector<std::tuple<int, int, int, int>>, std::greater<>>  pq;
	static std::vector<std::vector<bool>> visited = _canGo;
	static std::vector<std::vector<std::pair<int, int>>> trace(_canGo.size(), std::vector<std::pair<int, int>>(_canGo[0].size()));
	static int dy[4] = { 0,0,1,-1 };
	static int dx[4] = { 1,-1,0,0 };

	auto [startY, startX] = start;
	auto [destY, destX] = dest;
	while (!pq.empty())
		pq.pop();
	for (int i = 0; i < visited.size(); ++i)
		for (int j = 0; j < visited[i].size(); ++j)
			visited[i][j] = false;
	pq.push({ 0,0,startY, startX });
	visited[startY - _yMin][startX - _xMin] = true;
	while (!pq.empty())
	{
		auto [F, H, y, x] = pq.top();
		pq.pop();
		if (y == destY && x == destX)
		{
			while (true)
			{
				if (trace[y - _yMin][x - _xMin].first == startY && trace[y - _yMin][x - _xMin].second == startX)
					return std::pair<int, int>(y, x);
				std::tie(y, x) = trace[y - _yMin][x - _xMin];
			}
		}
		for (int i = 0; i < 4; ++i)
		{
			int ny = y + dy[i];
			int nx = x + dx[i];
			if (this->canGo(ny, nx) && !visited[ny - _yMin][nx - _xMin])
			{
				int G = std::abs(ny - startY) + std::abs(nx - startX);
				H = std::abs(ny - destY) + std::abs(nx - destX);
				F = G + H;
				pq.push({ F,H,ny,nx });
				visited[ny - _yMin][nx - _xMin] = true;
				trace[ny - _yMin][nx - _xMin] = { y,x };
			}
		}
	}
	return std::nullopt;
}
#pragma endregion

#pragma region private
Map::Map(const std::filesystem::path& path)
{
	std::ifstream mapFile(path);
	std::string line;
	mapFile >> _xMin >> _xMax >> _yMin >> _yMax;
	mapFile.ignore(::std::numeric_limits<std::streamsize>::max(), '\n');
	while (std::getline(mapFile, line))
	{
		std::vector<bool> v(line.size());
		for (int i = 0; i < line.size(); ++i)
		{
			if (line[i] == '0')
				v[i] = true;
			else
				v[i] = false;
		}
		_canGo.push_back(v);
	}
	_dat = new DAT();
	// TODO : basePoint 설정
	_basePointY = _basePointX = 2;
	MMOServerBroadcaster::onUpdate += [this]() { this->update(); };
}
Map::~Map() { delete _dat; }
void Map::update()
{
	for (GameObject* obj : _gameObjects)
		if (obj->activeInHierarchy())
			obj->broadcastToComponents(E_GameObjectEvent::UPDATE, nullptr);

	_dat->makeCandidates(_queryIDs, _candidates);
	this->updateCollisions();
	this->destroy();
	_queryIDs.clear();
	_candidates.clear();
}
void Map::updateCollisions()
{
	// delete invalid collisions.
	for (auto it = _collisions.begin(); it != _collisions.end();)
	{
		Area* areaA = it->first;
		Area* areaB = it->second;
		if (!LayerFilter::detectable(areaA->layer(), areaB->layer()) || !areaA->overlaps(*areaB) ||
			(areaA->gameObject() == areaB->gameObject() && !areaA->detectMyArea() && !areaB->detectMyArea()))
		{
			this->invokeAreaEvent(E_GameObjectEvent::AREA_EXIT, *it);
			it = _collisions.erase(it); // 여기서 삭제하고 ++it까지 돼서 그랬음.
			continue;
		}
		else
			++it;
	}

	for (int i = 0; i < _collisions.size(); ++i)
		this->invokeAreaEvent(E_GameObjectEvent::AREA_STAY, _collisions[i]);

	// import valid collisions.
	for (int i = 0; i < _candidates.size(); ++i)
	{
		Area* areaA = reinterpret_cast<Area*>(_dat->getData(_candidates[i].first));
		Area* areaB = reinterpret_cast<Area*>(_dat->getData(_candidates[i].second));
		if (!LayerFilter::detectable(areaA->layer(), areaB->layer()) || areaA == areaB ||
			(areaA->gameObject() == areaB->gameObject() && !areaA->detectMyArea() && !areaB->detectMyArea()))
			continue;
		const std::vector<Area*>& overlappedAreas = areaA->overlappedAreas();
		auto it = std::find(overlappedAreas.begin(), overlappedAreas.end(), areaB);
		if (it != overlappedAreas.end())
			continue;
		std::pair<Area*, Area*> collision = { areaA, areaB };
		this->invokeAreaEvent(E_GameObjectEvent::AREA_ENTER, collision);
		_collisions.push_back(collision);
	}
}
void Map::invokeAreaEvent(const E_GameObjectEvent& evt, const std::pair<Area*, Area*>& collision)
{
	std::pair<Area*, Area*> a = collision;
	std::pair<Area*, Area*> b = { collision.second, collision.first };
	collision.first->gameObject()->broadcastToComponents(evt, &a);
	collision.second->gameObject()->broadcastToComponents(evt, &b);
}
void Map::destroy()
{
	while (!_destroyedGameObjects.empty())
	{
		GameObject* obj = _destroyedGameObjects.front();
		_destroyedGameObjects.pop();
		delete obj;
	}
	while (!_destroyedComponents.empty())
	{
		Component* component = _destroyedComponents.front();
		_destroyedComponents.pop();
		delete component;
	}
}
int Map::add(void* userData, const AABB& aabb)
{
	int id = _dat->insert(userData, aabb);
	_queryIDs.push_back(id);
	return id;
}
void Map::remove(int id)
{
	{
		auto it = std::find(_queryIDs.begin(), _queryIDs.end(), id);
		if (it != _queryIDs.end())
			_queryIDs.erase(it);
	}
	Area* area = reinterpret_cast<Area*>(_dat->getData(id));
	_dat->remove(id);

	// collision 중에서 id에 해당하는 area와 관련된 것들 제거
	{
		auto it = _collisions.begin();
		while (it != _collisions.end())
		{
			it = std::find_if(it, _collisions.end(), [&area](const std::pair<Area*, Area*>& p) { return area == p.first || area == p.second; });
			if (it != _collisions.end())
			{
				this->invokeAreaEvent(E_GameObjectEvent::AREA_EXIT, *it);
				it = _collisions.erase(it);
			}
		}
	}
}
void Map::move(int id, const AABB& newAABB)
{
	auto it = std::find(_queryIDs.begin(), _queryIDs.end(), id);
	if (it == _queryIDs.end())
		_queryIDs.push_back(id);
	_dat->move(id, newAABB);
}
#pragma endregion


std::unordered_map<std::string, Map*> Map::s_maps;