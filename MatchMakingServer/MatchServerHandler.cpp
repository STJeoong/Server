#include "MatchServerHandler.h"
#include "match_protocol.pb.h"
#include "ProcessGenerator.h"
#include "Server.h"
#include <S_PacketHeader.h>

using namespace protocol::match;

#pragma region public
MatchServerHandler::MatchServerHandler() { _generator = new ProcessGenerator; }
MatchServerHandler::~MatchServerHandler() { delete _generator; }
void MatchServerHandler::handle(S_EngineEvent& evt)
{
	switch (evt.type)
	{
	case E_EngineEventType::EVENT_NET_CONNECT: printf("client connect : (%d)\n", evt.serial); return;
	case E_EngineEventType::EVENT_NET_DISCONNECT: printf("client disconnect : (%d)\n", evt.serial); return;
	}

	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	switch ((E_PacketID)header->id)
	{
	case E_PacketID::MATCH_REQ: this->matchReq(evt); break;
	case E_PacketID::MATCH_CANCLE_REQ: this->matchCancle(evt); break;
	}
}
#pragma endregion

#pragma region private
void MatchServerHandler::matchReq(S_EngineEvent& evt)
{
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	Match_Req req = {};
	Match_Resp resp = {};

	req.ParseFromArray(evt.data + sizeof(S_PacketHeader), header->initLen);
	if (_lastRegisteredSerial == MatchServerHandler::INVALID_SERIAL)
	{
		_lastRegisteredSerial = req.serial();
		return;
	}

	UINT16 portNum = _generator->generate();
	resp.add_serials(_lastRegisteredSerial);
	resp.add_serials(req.serial());
	resp.set_ip(Server::getInstance().getServerConfig().ip);
	resp.set_port(portNum);
	Server::getInstance().send(E_EngineType::MATCH_SERVER, evt.serial, { (UINT16)E_PacketID::MATCH_RESP, 0 }, resp);
	_lastRegisteredSerial = MatchServerHandler::INVALID_SERIAL;
}
void MatchServerHandler::matchCancle(S_EngineEvent& evt)
{
	S_PacketHeader* header = reinterpret_cast<S_PacketHeader*>(evt.data);
	MatchCancle_Req req = {};

	req.ParseFromArray(evt.data + sizeof(S_PacketHeader), header->initLen);
	if (_lastRegisteredSerial == req.serial())
		_lastRegisteredSerial = MatchServerHandler::INVALID_SERIAL;
}
#pragma endregion