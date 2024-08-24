#pragma once
#include <basetsd.h>
#include <google/protobuf/type.pb.h>
#include <string>
#include "E_EngineType.h"
#include "MMO_enum.pb.h"
#include "MMO_struct.pb.h"
namespace Utils
{
	// ���࿡ �ٸ� ���� �߰��Ǹ� ������ E_PacketID�� ���еǴϱ� engineID�� �����ʿ���� �����ε��ϸ� ��. ��� ���ӽ����̽��� �ٿ���ߵ�.
	void send(int serial, protocol::mmo::E_PacketID packetID, UINT8 option, const google::protobuf::Message& message);
	void send(int serial, protocol::mmo::E_PacketID packetID, UINT8 option);
	std::string eraseFileExtensionName(const std::string& fileName);
	// (���ڷ� �ִ�)local��ǥ�� (���ڷ� �ִ�)world��ǥ�� �������� ������ǥ�� ��ȯ.
	// �������� �������� �� ���.
	void localToWorld(const protocol::mmo::TransformInt& pivotWorld, const protocol::mmo::TransformInt& myLocal, protocol::mmo::TransformInt& myWorld);
	// (���ڷ� �ִ�)world��ǥ�� (���ڷ� �ִ�)world��ǥ�� �������� ������ǥ�� ��ȯ.
	// ��, �������� �״���ε� ���� ��ǥ�� �ٲ���� �� ���.
	void worldToLocal(const protocol::mmo::TransformInt& pivotWorld, const protocol::mmo::TransformInt& myWorld, protocol::mmo::TransformInt& myLocal);
	// �ӽ� (�Ҽ����� ���� Ȯ���� ó���ϰ� ����)
	bool gacha(int percentage);
}