#pragma once
#include <utility>
#include <basetsd.h>
#include <Windows.h>
class ProcessGenerator
{
	static const int MIN_PORT_NUM = 20000;
	static const int MAX_PORT_NUM = 21000;
public:
	ProcessGenerator();
	~ProcessGenerator();
	UINT16 generate();
private:
	std::pair<int, PROCESS_INFORMATION*> getUsableInfo();

	PROCESS_INFORMATION* _pinfos = nullptr;
};

