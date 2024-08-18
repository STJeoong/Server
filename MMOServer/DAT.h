#pragma once
#include <vector>
#include "AABB.h"

class DAT
{
    static const int INITIAL_STK_SIZE = 10000;
public:
    static const int NULL_NODE = -1; // 노드가 없음을 뜻함.

    DAT();
    int insert(void* userData, const AABB& aabb);
    void remove(int id);
    bool move(int id, const AABB& newAABB);
    void* getData(int id) const;
    const AABB& getAABB(int id) const;
    void makeCandidates(const std::vector<int>& queryId, std::vector<std::pair<int, int>>& candidates);
private:
    void insertLeaf(int leaf); // 균형을 유지하면서 새로운 leaf 노드를 적절한 위치에 위치시킴
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
    /// Node의 AABB는 4가지 상태 중 하나이다.
    /// 1. (group AABB, group AABB) 를 합치는 group AABB
    /// 2. (group AABB, instance AABB) 를 합치는 group AABB
    /// 3. (instance AABB, instance AABB) 를 합치는 group AABB
    /// 4. instance AABB(leaf node). 실제 객체를 가지고 있다.
    /// </summary>
    struct Node
    {
        bool isLeaf() const { return left == DAT::NULL_NODE; }
        AABB aabb; // 노드가 나타내는 AABB정보. 노드가 관리하는 공간의 경계를 뜻함.
        int parent; // 부모노드
        int left; // 왼쪽 자식노드
        int right; // 오른쪽 자식노드
        int height; // 현재 노드의 높이 정보. 높이는 depth의 반대라고 보면 됨.
        void* userData; // Object pointer or some user data
        bool moved;
    };
    std::vector<Node> _nodes; // 배열로 트리 관리
    int _root; // 루트 노드의 인덱스. 트리가 비어있으면 -1(NULL_NODE)로 초기화. 루트 노드는 모든 객체를 둘러싸는 group AABB를 가지고 있음.
    int _freeNode; // 할당된 노드가 삭제됐거나 할당되지 않았을 때 이 인덱스를 사용해서 새로운 노드 할당할 수 있음.
    int _cnt; // 현재 트리에 있는 노드의 갯수
    int _capacity; // 벡터 capacity

    std::vector<int> _stk; // for performance;
    int _stkCapacity;
};
