#pragma once
#include "I_UserState.h"
#include "S_UserInfo.h"

namespace userState
{
	class Matching : public I_UserState
	{
	public:
		Matching(S_UserInfo* info) { _userInfo = info; }
		virtual void onRun(void* args) override;
		virtual void onStop(void* args) override;
	private:
		S_UserInfo* _userInfo;
	};
}