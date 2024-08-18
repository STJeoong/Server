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
	void send(int serial, E_PacketID packetID, UINT8 option, const google::protobuf::Message& message);
	void send(int serial, protocol::mmo::E_PacketID packetID, UINT8 option);
	std::string eraseFileExtensionName(const std::string& fileName);
	// (인자로 주는)local좌표를 (인자로 주는)world좌표를 기준으로 월드좌표를 반환.
	// 기준점이 움직였을 때 사용.
	void localToWorld(const TransformInt& pivotWorld, const TransformInt& myLocal, TransformInt& myWorld);
	// (인자로 주는)world좌표를 (인자로 주는)world좌표를 기준으로 로컬좌표를 반환.
	// 즉, 기준점은 그대로인데 본인 좌표가 바뀌었을 때 사용.
	void worldToLocal(const TransformInt& pivotWorld, const TransformInt& myWorld, TransformInt& myLocal);
}