#include "Utils.h"
#include <Engine.h>
#include <MemoryBlockPool.h>
#include <S_PacketHeader.h>
#include "E_EngineType.h"
#include <random>

using namespace protocol::mmo;

void Utils::send(int serial, protocol::mmo::E_PacketID packetID, UINT8 option, const google::protobuf::Message& message)
{
	char* block = MemoryBlockPool::get((int)Size::_2048);
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(block);
	size_t messageSize = message.ByteSizeLong();

	header->id = (UINT16)packetID;
	header->option = option;
	header->initLen = sizeof(S_PacketHeader) + (UINT16)messageSize;
	header->len = sizeof(S_PacketHeader) + (UINT16)messageSize;

	//message.SerializeToArray(block + sizeof(S_PacketHeader), (int)Size::_2048 - (int)sizeof(S_PacketHeader));
	message.SerializeToArray(block + sizeof(S_PacketHeader), messageSize);
	Engine::send((int)E_EngineType::MMO_SERVER, serial, Size::_2048, block);
}
void Utils::send(int serial, protocol::mmo::E_PacketID packetID, UINT8 option)
{
	char* block = MemoryBlockPool::get((int)Size::_256);
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(block);

	header->id = (UINT16)packetID;
	header->option = option;
	header->initLen = sizeof(S_PacketHeader);
	header->len = sizeof(S_PacketHeader);

	Engine::send((int)E_EngineType::MMO_SERVER, serial, Size::_256, block);
}
std::string Utils::eraseFileExtensionName(const std::string& fileName)
{
	std::string ret = fileName;
	size_t idx = ret.find('.');
	return ret.substr(0, idx);
}
void Utils::localToWorld(const TransformInt& pivotWorld, const TransformInt& myLocal, TransformInt& myWorld)
{
	// 회전 행렬
	static int matrix[4][2][2] = { {{1,0},{0,1} }, {{0,-1},{1,0}}, {{-1,0},{0,-1} }, {{0,1},{-1,0}} };
	myWorld.set_dir((E_Dir)(((int)pivotWorld.dir() + (int)myLocal.dir()) % (int)E_Dir::MAX));
	myWorld.set_y(matrix[(int)pivotWorld.dir()][1][0] * myLocal.x() + matrix[(int)pivotWorld.dir()][1][1] * myLocal.y() + pivotWorld.y());
	myWorld.set_x(matrix[(int)pivotWorld.dir()][0][0] * myLocal.x() + matrix[(int)pivotWorld.dir()][0][1] * myLocal.y() + pivotWorld.x());
}
void Utils::worldToLocal(const TransformInt& pivotWorld, const TransformInt& myWorld, TransformInt& myLocal)
{
	// 회전 행렬의 역행렬
	static int invMat[4][2][2] = { {{1,0},{0,1}},{{0,1},{-1,0}},{{-1,0},{0,-1}},{{0,-1},{1,0}} };
	myLocal.set_dir((E_Dir)(((int)E_Dir::MAX - (int)pivotWorld.dir() + (int)myWorld.dir()) % (int)E_Dir::MAX));
	myLocal.set_y(invMat[(int)pivotWorld.dir()][1][0] * (myWorld.x() - pivotWorld.x()) + invMat[(int)pivotWorld.dir()][1][1] * (myWorld.y() - pivotWorld.y()));
	myLocal.set_x(invMat[(int)pivotWorld.dir()][0][0] * (myWorld.x() - pivotWorld.x()) + invMat[(int)pivotWorld.dir()][0][1] * (myWorld.y() - pivotWorld.y()));
}
bool Utils::gacha(int percentage)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution dis(1, 100);

	if (dis(gen) <= percentage) return true;
	return false;
}

int Utils::randomVal(int minVal, int maxVal)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution dis(minVal, maxVal);

	return dis(gen);
}

int Utils::createID(E_ObjectType objType, UINT8 templateID, int objID)
{
	int ret = 0; // all bits off
	ret |= ((int)objType) << 28;
	ret |= (int)templateID << 20;
	ret |= objID;
	return ret;
}

int Utils::getID(int id)
{
	int mask = (1 << 20) - 1;
	return id & mask;
}
