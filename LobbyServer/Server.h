#pragma once
#include "I_Server.h"
class PacketHandler;
class Server : public I_Server
{
public:
	virtual bool init() override; // DB addProtocol 등 등록 및 초기화 작업
	virtual void process(int serial, E_EngineEventType type, char* data) override;
private:
	PacketHandler* _packetHandler;
};