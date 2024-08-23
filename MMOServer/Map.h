#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <optional>
#include "MMO_struct.pb.h"

using namespace protocol::mmo;
class Map
{
public:
	static void load();
	static Map* getMap(const std::string& mapName);
private:
	static std::unordered_map<std::string, Map*> s_maps;


public:
	bool canGo(int y, int x);
	std::optional<std::pair<int, int>> findPath(const TransformInt& start, const TransformInt& dest);
private:
	Map(const std::filesystem::path& path);

	size_t _xMin;
	size_t _xMax;
	size_t _yMin;
	size_t _yMax;
	std::vector<std::vector<bool>> _canGo;
};

namespace MapName
{
	const std::string MAIN = "Main";
}