#pragma once
#include <algorithm>
#include <Define.h>
#include <S_PacketAttr.h>
#include <google/protobuf/type.pb.h>
class Serializer
{
public:
	static void serializeAndSend(int engineID, int serial, S_PacketAttr attr);
	static void serializeAndSend(int engineID, int serial, S_PacketAttr attr, const google::protobuf::Message& message);
private:
};

