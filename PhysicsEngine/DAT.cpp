#include "pch.h"
#include "DAT.h"

#pragma region public
DAT::DAT() : _root(DAT::NULL_NODE), _freeNode(0), _cnt(0), _capacity(16)
{
    _nodes.resize(_capacity);
    for (size_t i = 0; i < _capacity; ++i)
    {
        _nodes[i].parent = i + 1;
        _nodes[i].height = -1;
    }
    _nodes[_capacity - 1].parent = DAT::NULL_NODE;
}
size_t DAT::insert(void* userData, const AABB& aabb)
{
    size_t node = this->allocate();
    // fatten aabb
    Vector2D r{ DAT::FATTEN_AABB, DAT::FATTEN_AABB };
    _nodes[node].aabb += r;
    _nodes[node].aabb -= r;
    _nodes[node].userData = userData;
    this->insertLeaf(node);
    return node;
}
void DAT::remove(size_t leaf)
{
    this->removeLeaf(leaf);
    this->release(leaf);
}
bool DAT::move(size_t leaf, const AABB& sweepAABB, const Vector2D& displacement)
{
    Vector2D r{ DAT::FATTEN_AABB, DAT::FATTEN_AABB };
    AABB fatAABB = sweepAABB;
    fatAABB += r;
    fatAABB -= r;

    Vector2D predictMovement = displacement * DAT::PREDICT_MULTIPLIER;
    fatAABB += predictMovement;

    const AABB& treeAABB = _nodes[leaf].aabb;
    if (treeAABB.contains(sweepAABB))
    {
        AABB hugeAABB = fatAABB;
        hugeAABB += r * 4.0f;
        hugeAABB -= r * 4.0f;
        if (hugeAABB.contains(treeAABB)) return false;
        // ex) 사각형 오브젝트가 초기에 45도 기울어진 상태로 있음. => 기존 오브젝트보다 1.41배(루트2)만큼 큰 treeAABB 잡힘
        // rotation이 0이 되고 그 상태에서 살짝 움직임.
        // 이 상황에서는 hugeAABB가 treeAABB보다 작을 수 있음. => treeAABB 크기 재조정 필요
    }
    this->removeLeaf(leaf);
    _nodes[leaf].aabb = fatAABB;
    this->insertLeaf(leaf);
    return true;
}
#pragma endregion

#pragma region private
void DAT::insertLeaf(size_t leaf)
{
    if (_root == DAT::NULL_NODE)
    {
        _root = leaf;
        _nodes[_root].parent = DAT::NULL_NODE;
    }

    size_t sibling = this->findBestSibling(leaf);
    size_t oldParent = _nodes[sibling].parent;
    size_t newParent = this->allocate();
    _nodes[newParent].parent = oldParent;
    _nodes[newParent].userData = nullptr;
    _nodes[newParent].aabb = _nodes[leaf].aabb + _nodes[sibling].aabb;
    _nodes[newParent].height = _nodes[sibling].height + 1;
    _nodes[newParent].left = sibling;
    _nodes[newParent].right = leaf;
    _nodes[sibling].parent = newParent;
    _nodes[leaf].parent = newParent;

    if (oldParent != DAT::NULL_NODE)
    {
        if (_nodes[oldParent].left == sibling)
            _nodes[oldParent].left = newParent;
        else
            _nodes[oldParent].right = newParent;
    }
    else
        _root = newParent;
    this->fixUpwardsTree(newParent);
}
void DAT::removeLeaf(size_t leaf)
{
    if (leaf == _root)
    {
        _root = DAT::NULL_NODE;
        return;
    }

    size_t parent = _nodes[leaf].parent;
    size_t grandParent = _nodes[parent].parent;
    size_t sibling;
    if (_nodes[parent].left == leaf)
        sibling = _nodes[parent].right;
    else
        sibling = _nodes[parent].left;
    _nodes[sibling].parent = grandParent;
    this->release(parent);

    if (grandParent == DAT::NULL_NODE)
    {
        _root = sibling;
        return;
    }
    if (_nodes[grandParent].left == parent)
        _nodes[grandParent].left = sibling;
    else
        _nodes[grandParent].right = sibling;
    this->fixUpwardsTree(grandParent);
}
size_t DAT::findBestSibling(size_t leaf)
{
    size_t idx = _root;
    while (!_nodes[idx].isLeaf()) // node != leaf 면 left, right모두 -1이 아님. 반대로 node == leaf면 둘다 -1임.
    {
        size_t left = _nodes[idx].left;
        size_t right = _nodes[idx].right;

        float area = _nodes[idx].aabb.perimeter();
        AABB combinedAABB = _nodes[idx].aabb + _nodes[leaf].aabb;
        float combinedArea = combinedAABB.perimeter();
        float cost = 2.0f * combinedArea; // 새로운 group AABB를 만드는 비용 ( 현재 노드가 나타내는 그룹과 새로운 리프노드가 합쳐져서 새로운 그룹을 생성 ). 왜 2를 곱하는 진 모르겠음.
        float inheritanceCost = 2.0f * (combinedArea - area); // 기존의 group AABB를 확장하는 비용 ( 왼쪽 or 오른쪽 자식 그룹에 리프노드 추가 )

        float leftCost = inheritanceCost + this->cost(left, _nodes[leaf].aabb); // 왼쪽 자식 그룹에 추가할 비용
        float rightCost = inheritanceCost + this->cost(right, _nodes[leaf].aabb); // 오른쪽 자식 그룹에 추가할 비용

        if (cost < leftCost && cost < rightCost) break;
        idx = leftCost < rightCost ? left : right;
    }
    return idx;
}
void DAT::fixUpwardsTree(size_t idx)
{
    while (idx != DAT::NULL_NODE)
    {
        idx = this->balance(idx);

        int left = _nodes[idx].left;
        int right = _nodes[idx].right;

        _nodes[idx].height = 1 + max(_nodes[left].height, _nodes[right].height);
        _nodes[idx].aabb = _nodes[left].aabb + _nodes[right].aabb;

        idx = _nodes[idx].parent;
    }
}
size_t DAT::balance(size_t iA)
{
    if (_nodes[iA].isLeaf() || _nodes[iA].height < 2) return iA;
    size_t iB = _nodes[iA].left;
    size_t iC = _nodes[iA].right;
    int balance = _nodes[iB].height - _nodes[iC].height;
    if (balance < -1) // R
    {
        size_t iF = _nodes[iC].left;
        size_t iG = _nodes[iC].right;
        if (_nodes[iF].height > _nodes[iG].height) // RL 상태 : LL -> RR
        {
            this->ll(iC);
            this->rr(iA);
            return iF;
        }
        else // RR 상태 : RR
        {
            this->rr(iA);
            return iC;
        }
    }
    if (balance > 1) // L
    {
        size_t iD = _nodes[iB].left;
        size_t iE = _nodes[iB].right;
        if (_nodes[iD].height > _nodes[iE].height) // LL 상태
        {
            this->ll(iA);
            return iB;
        }
        else // LR 상태 : RR -> LL
        {
            this->rr(iB);
            this->ll(iA);
            return iE;
        }
    }
    return iA;
}
void DAT::rr(size_t iA)
{
    size_t iC = _nodes[iA].right;
    size_t iF = _nodes[iC].left;
    size_t iG = _nodes[iC].right;
    size_t oldParent = _nodes[iA].parent;
    Node& A = _nodes[iA];
    Node& C = _nodes[iC];
    Node& F = _nodes[iF];
    Node& G = _nodes[iG];

    C.aabb = A.aabb;
    C.parent = oldParent;
    C.left = iA;
    A.parent = iC;
    A.right = iF;
    F.parent = iA; // 이걸 안해줘서 무한루프 돌았음.
    A.aabb = _nodes[A.left].aabb + F.aabb;
    A.height = max(_nodes[A.left].height, F.height) + 1;
    C.height = max(A.height, G.height) + 1;

    if (oldParent == DAT::NULL_NODE)
        _root = iC;
    else
    {
        if (_nodes[oldParent].left == iA)
            _nodes[oldParent].left = iC;
        else
            _nodes[oldParent].right = iC;
    }
}
void DAT::ll(size_t iA)
{
    size_t iB = _nodes[iA].left;
    size_t iD = _nodes[iB].left;
    size_t iE = _nodes[iB].right;
    size_t oldParent = _nodes[iA].parent;
    Node& A = _nodes[iA];
    Node& B = _nodes[iB];
    Node& D = _nodes[iD];
    Node& E = _nodes[iE];

    B.aabb = A.aabb;
    B.parent = oldParent;
    A.left = iE;
    E.parent = iA; // 이걸 안해줬었음.
    A.parent = iB;
    B.right = iA;
    A.aabb = _nodes[A.right].aabb + E.aabb;
    A.height = max(_nodes[A.right].height, E.height) + 1;
    B.height = max(A.height, D.height) + 1;

    if (oldParent == DAT::NULL_NODE)
        _root = iB;
    else
    {
        if (_nodes[oldParent].left == iA)
            _nodes[oldParent].left = iB;
        else
            _nodes[oldParent].right = iB;
    }
}
size_t DAT::allocate()
{
    if (_freeNode == DAT::NULL_NODE)
    {
        size_t oldCapacity = _capacity;
        _capacity *= 2;
        _nodes.resize(_capacity);
        for (size_t i = oldCapacity; i < _capacity; ++i)
        {
            _nodes[i].parent = i + 1;
            _nodes[i].height = -1;
        }
        _nodes[_capacity - 1].parent = DAT::NULL_NODE;
        _freeNode = oldCapacity;
    }
    size_t node = _freeNode;
    _freeNode = _nodes[node].parent; // _freeNode를 다음 미사용중인 노드로 옮김
    _nodes[node].parent = DAT::NULL_NODE;
    _nodes[node].left = DAT::NULL_NODE;
    _nodes[node].right = DAT::NULL_NODE;
    _nodes[node].height = 0;
    ++_cnt;
    return node;
}
void DAT::release(size_t node)
{
    _nodes[node].parent = _freeNode;
    _nodes[node].height = -1;
    _freeNode = node;
    --_cnt;
}
float DAT::cost(size_t i, const AABB& aabb) const
{
    AABB combinedAABB = _nodes[i].aabb + aabb;
    float combinedArea = combinedAABB.perimeter();

    float cost = combinedArea;
    if (!_nodes[i].isLeaf())
    {
        float oldArea = _nodes[i].aabb.perimeter();
        cost = (combinedArea - oldArea);
    }

    return cost;
}
#pragma endregion

const float DAT::FATTEN_AABB = 0.1f;
const float DAT::PREDICT_MULTIPLIER = 4.0f;