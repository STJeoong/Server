#pragma once
#include <basetsd.h>
#include <google/protobuf/type.pb.h>
#include <string>
#include "E_EngineType.h"
#include "MMO_enum.pb.h"
#include "MMO_struct.pb.h"
#include "S_RectDefine.h"
#include "AABB.h"
namespace Utils
{
	// 만약에 다른 엔진 추가되면 어차피 E_PacketID로 구분되니까 engineID는 받을필요없이 오버로딩하면 됨. 대신 네임스페이스는 붙여줘야됨.
	void send(int serial, protocol::mmo::E_PacketID packetID, UINT8 option, const google::protobuf::Message& message);
	void send(int serial, protocol::mmo::E_PacketID packetID, UINT8 option);
	std::string eraseFileExtensionName(const std::string& fileName);
	// (인자로 주는)local좌표를 (인자로 주는)world좌표를 기준으로 월드좌표를 반환.
	// 기준점이 움직였을 때 사용.
	void localToWorld(const protocol::mmo::TransformInt& pivotWorld, const protocol::mmo::TransformInt& myLocal, protocol::mmo::TransformInt& myWorld);
	// (인자로 주는)world좌표를 (인자로 주는)world좌표를 기준으로 로컬좌표를 반환.
	// 즉, 기준점은 그대로인데 본인 좌표가 바뀌었을 때 사용.
	void worldToLocal(const protocol::mmo::TransformInt& pivotWorld, const protocol::mmo::TransformInt& myWorld, protocol::mmo::TransformInt& myLocal);
	// 임시 (소수점이 없는 확률만 처리하고 있음)
	bool gacha(int percentage);
	int randomVal(int minVal, int maxVal);
	int createID(protocol::mmo::E_ObjectType objType, UINT8 templateID, int objID);
	int getTemplateID(int id);
	int getID(int id);
	AABB calcAABB(const S_RectDefine& rect, const protocol::mmo::TransformInt& pivot, bool fixedRotation);
	// 캐릭터를 기준으로하는 AABB를 생성할때 사용
	AABB calcAdjustedAABB(const S_RectDefine& rect, const protocol::mmo::TransformInt& pivot, bool fixedRotation);
}
const int dy[4] = { 0,1,0,-1 };
const int dx[4] = { 1,0,-1,0 };