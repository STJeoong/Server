#include <vector>
#include "AABB.h"

class DAT
{
    static const int NULL_NODE = -1; // 노드가 없음을 뜻함.
    static const float FATTEN_AABB;
    static const float PREDICT_MULTIPLIER;
public:
    DAT();
    size_t insert(void* userData, const AABB& aabb);
    void remove(size_t leaf);
    bool move(size_t leaf, const AABB& sweepAABB, const Vector2D& displacement); // sweepAABB = 물체의 시작위치AABB + 물체의 마지막위치AABB, displacement : 변위
private:
    void insertLeaf(size_t leaf); // 균형을 유지하면서 새로운 leaf 노드를 적절한 위치에 위치시킴
    void removeLeaf(size_t leaf);
    size_t findBestSibling(size_t leaf);
    void fixUpwardsTree(size_t idx);
    size_t balance(size_t iA);
    void rr(size_t iA);
    void ll(size_t iA);
    size_t allocate();
    void release(size_t node);
    float cost(size_t i, const AABB& aabb) const;

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
        size_t parent; // 부모노드
        size_t left; // 왼쪽 자식노드
        size_t right; // 오른쪽 자식노드
        int height; // 현재 노드의 높이 정보. 높이는 depth의 반대라고 보면 됨.
        void* userData; // Object pointer or some user data
    };
    std::vector<Node> _nodes; // 배열로 트리 관리
    size_t _root; // 루트 노드의 인덱스. 트리가 비어있으면 -1(NULL_NODE)로 초기화. 루트 노드는 모든 객체를 둘러싸는 group AABB를 가지고 있음.
    size_t _freeNode; // 할당된 노드가 삭제됐거나 할당되지 않았을 때 이 인덱스를 사용해서 새로운 노드 할당할 수 있음.
    size_t _cnt; // 현재 트리에 있는 노드의 갯수
    size_t _capacity; // 벡터 capacity
};
