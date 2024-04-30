#pragma once
#include <vector>
#include <tuple>
#include "DAT.h"
class AABB;
// ���� ������ ���ؼ� �浹���ɼ� �ִ� ���� ��ȯ.
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
	std::vector<std::pair<int, int>> _candidates; // �浹���ɼ� �ִ� �ĺ��� �ֵ�
	std::vector<int> _thingsToBeUpdated; // �����̰ų� �߰��� ��� id����
};