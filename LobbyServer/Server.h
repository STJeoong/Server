#pragma once
#include "I_Server.h"
class PacketHandler;
class Server : public I_Server
{
public:
	virtual bool init() override; // DB addProtocol �� ��� �� �ʱ�ȭ �۾�
	virtual void process(int serial, E_EngineEvent type, char* data) override;
private:
	PacketHandler* _packetHandler;
};