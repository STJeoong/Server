#include "Map.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include "Utils.h"

#pragma region public
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
		s_maps[Utils::eraseFileExtensionName(entry.path().filename().string())] = new Map(entry.path());
}
Map* Map::getMap(const std::string& mapName) { return s_maps[mapName]; }


bool Map::canGo(int y, int x)
{
	if (y < _yMin || y > _yMax || x < _xMin || x > _xMax)
		return false;
	return _canGo[y - _yMin][x - _xMin];
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