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

using namespace protocol::mmo;
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
void Map::overlapsBox(const AABB& box, int filter, std::vector<GameObject*>& list)
{
	_dat->overlapsBox(box, filter, list);
}
bool Map::canGo(int y, int x)
{
	if (y < _yMin || y > _yMax || x < _xMin || x > _xMax)
		return false;
	return _canGo[y - _yMin][x - _xMin];
}
std::optional<E_Dir> Map::findPath(const TransformInt& start, const TransformInt& dest)
{
	static std::priority_queue<std::tuple<int, int, int, int>, std::vector<std::tuple<int, int, int, int>>, std::greater<>>  pq;
	static std::vector<std::vector<bool>> visited = _canGo;
	static std::vector<std::vector<E_Dir>> trace(_canGo.size(), std::vector<E_Dir>(_canGo[0].size()));

	int startY = start.y(), startX = start.x();
	int destY = dest.y(), destX = dest.x();
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
				int dir = ((int)trace[y - _yMin][x - _xMin] + ((int)E_Dir::DIR_MAX / 2)) % (int)E_Dir::DIR_MAX; // 반대 방향
				int ny = y + dy[dir]; // 이전 y좌표
				int nx = x + dx[dir]; // 이전 x좌표
				if (ny == startY && nx == startX)
					return trace[y - _yMin][x - _xMin];
				y = ny;
				x = nx;
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
				trace[ny - _yMin][nx - _xMin] = (E_Dir)i; // 이전 위치에서 해당 방향으로 이동헀음.
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
	std::queue<std::pair<int, int>> q; // 맵 바깥쪽의 0을 1로 바꾸기 위해서
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
	// 맵 아웃라인을 감싸는 장애물밖에 있는 0 전부 1로 변경
	if (_canGo[0][0])
		q.push({ _yMin,_xMin });
	else if (_canGo[0][_xMax - _xMin])
		q.push({ _yMin, _xMax });
	else if (_canGo[_yMax - _yMin][0])
		q.push({ _yMax, _xMin });
	else
		q.push({ _yMax, _xMax });
	_canGo[0][0] = _canGo[0][_xMax - _xMin] = _canGo[_yMax - _yMin][0] = _canGo[_yMax - _yMin][_xMax - _xMin] = false;
	while (!q.empty())
	{
		auto [y, x] = q.front();
		q.pop();
		
		for (int i = 0; i < 4; ++i)
		{
			int ny = y + dy[i];
			int nx = x + dx[i];
			if (this->canGo(ny, nx))
			{
				q.push({ ny,nx });
				_canGo[ny - _yMin][nx - _xMin] = false;
			}
		}
	}
	_dat = new DAT();
	// TODO : basePoint 설정
	_basePointY = _basePointX = 0;
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
		Area* areaA = _dat->getData(_candidates[i].first);
		Area* areaB = _dat->getData(_candidates[i].second);
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
int Map::add(Area* userData, const AABB& aabb)
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
	Area* area = _dat->getData(id);
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