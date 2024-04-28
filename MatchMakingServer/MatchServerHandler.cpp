#include "MatchServerHandler.h"
#include "match_protocol.pb.h"
#include "ProcessGenerator.h"
#include "MatchServerBroadcaster.h"
#include "E_EngineType.h"
#include "Serializer.h"
#include <ThreadPool.h>
#include <S_PacketHeader.h>
#include <Engine.h>

using namespace protocol::match;

#pragma region public
MatchServerHandler::MatchServerHandler()
{
	_generator = new ProcessGenerator;
	MatchServerBroadcaster::onConnect(true, [](int serial) { printf("client(%d) connected\n", serial); });
	MatchServerBroadcaster::onDisconnect(true, [](int serial) { printf("client(%d) disconnected\n", serial); });
	MatchServerBroadcaster::onMatchReq(true, std::bind(&MatchServerHandler::onMatchReq, this, std::placeholders::_1, std::placeholders::_2));
	MatchServerBroadcaster::onMatchCancleReq(true, std::bind(&MatchServerHandler::onMatchCancle, this, std::placeholders::_1, std::placeholders::_2));
}
MatchServerHandler::~MatchServerHandler()
{
	delete _generator;
	MatchServerBroadcaster::onMatchReq(false, std::bind(&MatchServerHandler::onMatchReq, this, std::placeholders::_1, std::placeholders::_2));
	MatchServerBroadcaster::onMatchCancleReq(false, std::bind(&MatchServerHandler::onMatchCancle, this, std::placeholders::_1, std::placeholders::_2));
}
#pragma endregion

#pragma region private
void MatchServerHandler::onMatchReq(int serial, const Match_Req& req)
{
	if (_lastRegisteredSerial == MatchServerHandler::INVALID_SERIAL)
	{
		_lastRegisteredSerial = req.serial();
		return;
	}

	int s1 = req.serial(), s2 = _lastRegisteredSerial;
	_lastRegisteredSerial = MatchServerHandler::INVALID_SERIAL;
	ThreadPool::enqueue([serial, this, s1, s2]() {
		UINT16 portNum = _generator->generate();
		Match_Resp resp = {};
		resp.add_serials(s1);
		resp.add_serials(s2);
		resp.set_ip(Engine::getEngineConfig((int)E_EngineType::MATCH_SERVER).ip);
		resp.set_port(portNum);
		std::pair<Size, char*> ret = Serializer::serialize({ (UINT16)E_PacketID::MATCH_RESP, 0 }, resp);
		Engine::send((int)E_EngineType::MATCH_SERVER, serial, ret.first, ret.second);});
}
void MatchServerHandler::onMatchCancle(int serial, const MatchCancle_Req& req)
{
	if (_lastRegisteredSerial == req.serial())
		_lastRegisteredSerial = MatchServerHandler::INVALID_SERIAL;
}
#pragma endregion