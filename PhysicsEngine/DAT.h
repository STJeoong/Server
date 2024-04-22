#include <vector>
#include "AABB.h"

class DAT
{
    static const int NULL_NODE = -1; // ��尡 ������ ����.
    static const float FATTEN_AABB;
    static const float PREDICT_MULTIPLIER;
public:
    DAT();
    size_t insert(void* userData, const AABB& aabb);
    void remove(size_t leaf);
    bool move(size_t leaf, const AABB& sweepAABB, const Vector2D& displacement); // sweepAABB = ��ü�� ������ġAABB + ��ü�� ��������ġAABB, displacement : ����
private:
    void insertLeaf(size_t leaf); // ������ �����ϸ鼭 ���ο� leaf ��带 ������ ��ġ�� ��ġ��Ŵ
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
        size_t parent; // �θ���
        size_t left; // ���� �ڽĳ��
        size_t right; // ������ �ڽĳ��
        int height; // ���� ����� ���� ����. ���̴� depth�� �ݴ��� ���� ��.
        void* userData; // Object pointer or some user data
    };
    std::vector<Node> _nodes; // �迭�� Ʈ�� ����
    size_t _root; // ��Ʈ ����� �ε���. Ʈ���� ��������� -1(NULL_NODE)�� �ʱ�ȭ. ��Ʈ ���� ��� ��ü�� �ѷ��δ� group AABB�� ������ ����.
    size_t _freeNode; // �Ҵ�� ��尡 �����ưų� �Ҵ���� �ʾ��� �� �� �ε����� ����ؼ� ���ο� ��� �Ҵ��� �� ����.
    size_t _cnt; // ���� Ʈ���� �ִ� ����� ����
    size_t _capacity; // ���� capacity
};
