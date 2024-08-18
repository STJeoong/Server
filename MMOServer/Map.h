#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include "MMO_struct.pb.h"

using namespace protocol::mmo;
class Map
{
public:
	static void load();
	static Map* getMap(const std::string& mapName);

	bool canGo(int y, int x);
	bool findPath(int startY, int startX, int destY, int destX, int& resultY, int& resultX);
private:
	Map(const std::filesystem::path& path);

	static std::unordered_map<std::string, Map*> s_maps;

	int _xMin;
	int _xMax;
	int _yMin;
	int _yMax;
	std::vector<std::vector<bool>> _canGo;
};