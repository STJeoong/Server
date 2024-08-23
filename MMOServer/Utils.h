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
	// ���࿡ �ٸ� ���� �߰��Ǹ� ������ E_PacketID�� ���еǴϱ� engineID�� �����ʿ���� �����ε��ϸ� ��. ��� ���ӽ����̽��� �ٿ���ߵ�.
	void send(int serial, E_PacketID packetID, UINT8 option, const google::protobuf::Message& message);
	void send(int serial, E_PacketID packetID, UINT8 option);
	std::string eraseFileExtensionName(const std::string& fileName);
	// (���ڷ� �ִ�)local��ǥ�� (���ڷ� �ִ�)world��ǥ�� �������� ������ǥ�� ��ȯ.
	// �������� �������� �� ���.
	void localToWorld(const TransformInt& pivotWorld, const TransformInt& myLocal, TransformInt& myWorld);
	// (���ڷ� �ִ�)world��ǥ�� (���ڷ� �ִ�)world��ǥ�� �������� ������ǥ�� ��ȯ.
	// ��, �������� �״���ε� ���� ��ǥ�� �ٲ���� �� ���.
	void worldToLocal(const TransformInt& pivotWorld, const TransformInt& myWorld, TransformInt& myLocal);
	// �ӽ� (�Ҽ����� ���� Ȯ���� ó���ϰ� ����)
	bool gacha(int percentage);
}