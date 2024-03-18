#pragma once
#include <algorithm>
#include <Define.h>
#include <S_PacketAttr.h>
#include <google/protobuf/type.pb.h>
class Serializer
{
public:
	std::pair<Size, char*> serialize(S_PacketAttr attr);
	std::pair<Size, char*> serialize(S_PacketAttr attr, const google::protobuf::Message& message);
private:
};

