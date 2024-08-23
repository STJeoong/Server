#pragma once
#include <basetsd.h>
#include <google/protobuf/type.pb.h>
#include <string>
#include "E_EngineType.h"
#include "MMO_enum.pb.h"
#include "MMO_struct.pb.h"
using namespace protocol::mmo;
namespace Utils
{
	// 만약에 다른 엔진 추가되면 어차피 E_PacketID로 구분되니까 engineID는 받을필요없이 오버로딩하면 됨. 대신 네임스페이스는 붙여줘야됨.
	void send(int serial, E_PacketID packetID, UINT8 option, const google::protobuf::Message& message);
	void send(int serial, E_PacketID packetID, UINT8 option);
	std::string eraseFileExtensionName(const std::string& fileName);
	// (인자로 주는)local좌표를 (인자로 주는)world좌표를 기준으로 월드좌표를 반환.
	// 기준점이 움직였을 때 사용.
	void localToWorld(const TransformInt& pivotWorld, const TransformInt& myLocal, TransformInt& myWorld);
	// (인자로 주는)world좌표를 (인자로 주는)world좌표를 기준으로 로컬좌표를 반환.
	// 즉, 기준점은 그대로인데 본인 좌표가 바뀌었을 때 사용.
	void worldToLocal(const TransformInt& pivotWorld, const TransformInt& myWorld, TransformInt& myLocal);
	// 임시 (소수점이 없는 확률만 처리하고 있음)
	bool gacha(int percentage);
}