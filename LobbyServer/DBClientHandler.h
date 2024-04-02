#pragma once
#include <S_EngineEvent.h>
#include "db_protocol.pb.h"
class DBClientHandler
{
public:
	DBClientHandler();
	~DBClientHandler();
private:
	void onLoginResp(const protocol::db::Login_Resp& resp);
};

