#pragma once
#include <S_EngineEvent.h>

class ProcessGenerator;
class MatchServerHandler
{
	static const int INVALID_SERIAL = -1;
public:
	MatchServerHandler();
	~MatchServerHandler();
	void handle(S_EngineEvent& evt);
private:
	void matchReq(S_EngineEvent& evt);
	void matchCancle(S_EngineEvent& evt);

	int _lastRegisteredSerial = MatchServerHandler::INVALID_SERIAL;
	ProcessGenerator* _generator;
};

