#pragma once
#include <vector>
#include "AABB.h"

class DAT
{
    static const int INITIAL_STK_SIZE = 10000;
public:
    static const int NULL_NODE = -1; // ��尡 ������ ����.

    DAT();
    int insert(void* userData, const AABB& aabb);
    void remove(int id);
    bool move(int id, const AABB& newAABB);
    void* getData(int id) const;
    const AABB& getAABB(int id) const;
    void makeCandidates(const std::vector<int>& queryId, std::vector<std::pair<int, int>>& candidates);
private:
    void insertLeaf(int leaf); // ������ �����ϸ鼭 ���ο� leaf ��带 ������ ��ġ�� ��ġ��Ŵ
    void removeLeaf(int leaf);
    int findBestSibling(int leaf);
    void fixUpwards(int idx);
    int balance(int iA);
    void rr(int iA);
    void ll(int iA);
    int allocate();
    void release(int node);
    int cost(int i, const AABB& aabb) const;

    /// <summary>
    /// Node�� AABB�� 4���� ���� �� �ϳ��̴�.
    /// 1. (group AABB, group AABB) �� ��ġ�� group AABB
    /// 2. (group AABB, instance AABB) �� ��ġ�� group AABB
    /// 3. (instance AABB, instance AABB) �� ��ġ�� group AABB
    /// 4. instance AABB(leaf node). ���� ��ü�� ������ �ִ�.
    /// </summary>
    struct Node
    {
        bool isLeaf() const { return left == DAT::NULL_NODE; }
        AABB aabb; // ��尡 ��Ÿ���� AABB����. ��尡 �����ϴ� ������ ��踦 ����.
        int parent; // �θ���
        int left; // ���� �ڽĳ��
        int right; // ������ �ڽĳ��
        int height; // ���� ����� ���� ����. ���̴� depth�� �ݴ��� ���� ��.
        void* userData; // Object pointer or some user data
        bool moved;
    };
    std::vector<Node> _nodes; // �迭�� Ʈ�� ����
    int _root; // ��Ʈ ����� �ε���. Ʈ���� ��������� -1(NULL_NODE)�� �ʱ�ȭ. ��Ʈ ���� ��� ��ü�� �ѷ��δ� group AABB�� ������ ����.
    int _freeNode; // �Ҵ�� ��尡 �����ưų� �Ҵ���� �ʾ��� �� �� �ε����� ����ؼ� ���ο� ��� �Ҵ��� �� ����.
    int _cnt; // ���� Ʈ���� �ִ� ����� ����
    int _capacity; // ���� capacity

    std::vector<int> _stk; // for performance;
    int _stkCapacity;
};
