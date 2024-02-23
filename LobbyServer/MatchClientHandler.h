#pragma once
#include <S_EngineEvent.h>
class MatchClientHandler
{
public:
	void handle(S_EngineEvent& evt);
private:
	void matchResp(S_EngineEvent& evt);
};