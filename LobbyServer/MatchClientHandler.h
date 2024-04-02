#pragma once
#include <S_EngineEvent.h>
#include "match_protocol.pb.h"
class MatchClientHandler
{
public:
	MatchClientHandler();
	~MatchClientHandler();
private:
	void onMatchResp(const protocol::match::Match_Resp& resp);
};