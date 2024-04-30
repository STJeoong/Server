#pragma once
#include <vector>
#include <tuple>
#include "DAT.h"
class AABB;
// 들어온 쿼리에 대해서 충돌가능성 있는 쌍을 반환.
class BroadPhase
{
public:
	int insert(void* userData, const AABB& aabb);
	void remove(int id);
	bool move(int id, const AABB& sweepAABB, const Vector2D& displacement);
	void* getData(int id) const { return _tree.getData(id); }
	const AABB& getAABB(int id) const { return _tree.getAABB(id); }
	void update();
	const std::vector<std::pair<int, int>>& candidates() const { return _candidates; }
private:
	DAT _tree;
	std::vector<std::pair<int, int>> _candidates; // 충돌가능성 있는 후보군 쌍들
	std::vector<int> _thingsToBeUpdated; // 움직이거나 추가된 노드 id값들
};