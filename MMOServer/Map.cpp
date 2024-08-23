#include "Map.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <queue>
#include "MMO_struct.pb.h"
#include "Utils.h"
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
bool Map::canGo(int y, int x)
{
	if (y < _yMin || y > _yMax || x < _xMin || x > _xMax)
		return false;
	return _canGo[y - _yMin][x - _xMin];
}
std::optional<std::pair<int, int>> Map::findPath(const TransformInt& start, const TransformInt& dest)
{
	static std::priority_queue<std::tuple<int, int, int, int>, std::vector<std::tuple<int, int, int, int>>, std::greater<>>  pq;
	static std::vector<std::vector<bool>> visited = _canGo;
	static std::vector<std::vector<std::pair<int, int>>> trace(_canGo.size(), std::vector<std::pair<int, int>>(_canGo[0].size()));
	static int dy[4] = { 0,0,1,-1 };
	static int dx[4] = { 1,-1,0,0 };

	while (!pq.empty())
		pq.pop();
	for (int i = 0; i < visited.size(); ++i)
		for (int j = 0; j < visited[i].size(); ++j)
			visited[i][j] = false;
	pq.push({ 0,0,start.y(), start.x() });
	visited[start.y()][start.x()] = true;
	while (!pq.empty())
	{
		auto [F, H, y, x] = pq.top();
		pq.pop();
		if (y == dest.y() && x == dest.x())
		{
			while (true)
			{
				if (trace[y][x].first == start.y() && trace[y][x].second == start.x())
					return std::pair<int, int>(y, x);
				std::tie(y, x) = trace[y][x];
			}
		}
		for (int i = 0; i < 4; ++i)
		{
			int ny = y + dy[i];
			int nx = x + dx[i];
			if (this->canGo(ny, nx) && !visited[ny][nx])
			{
				int G = std::abs(ny - start.y()) + std::abs(nx - start.x());
				H = std::abs(ny - dest.y()) + std::abs(nx - dest.x());
				F = G + H;
				pq.push({ F,H,ny,nx });
				visited[ny][nx] = true;
				trace[ny][nx] = { y,x };
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
	mapFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
}
#pragma endregion


std::unordered_map<std::string, Map*> Map::s_maps;