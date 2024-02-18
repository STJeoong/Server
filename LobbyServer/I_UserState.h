#pragma once
class I_UserState
{
public:
	virtual void onRun(void* args) = 0;
	virtual void onStop(void* args) = 0;
};