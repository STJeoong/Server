#pragma once
#include <S_EngineEvent.h>
#include "match_protocol.pb.h"

using namespace protocol::match;
class ProcessGenerator;
class MatchServerHandler
{
	static const int INVALID_SERIAL = -1;
public:
	MatchServerHandler();
	~MatchServerHandler();
private:
	void onMatchReq(int serial, const Match_Req& req);
	void onMatchCancle(int serial, const MatchCancle_Req& req);

	int _lastRegisteredSerial = MatchServerHandler::INVALID_SERIAL;
	ProcessGenerator* _generator;
};

