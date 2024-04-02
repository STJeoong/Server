#include "ProcessGenerator.h"
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <Engine.h>
#include <iostream>
#include "E_EngineType.h"

#pragma region public
ProcessGenerator::ProcessGenerator()
{
	_pinfos = new PROCESS_INFORMATION[ProcessGenerator::MAX_PORT_NUM - ProcessGenerator::MIN_PORT_NUM + 1]{};
	for (int i = 0; i <= ProcessGenerator::MAX_PORT_NUM - ProcessGenerator::MIN_PORT_NUM; ++i)
		_pinfos[i].hProcess = INVALID_HANDLE_VALUE;
}
ProcessGenerator::~ProcessGenerator()
{
	for (int i = ProcessGenerator::MIN_PORT_NUM; i < ProcessGenerator::MAX_PORT_NUM; ++i)
		if (_pinfos[i].hProcess != INVALID_HANDLE_VALUE)
		{
			CloseHandle(_pinfos[i].hThread);
			CloseHandle(_pinfos[i].hProcess);
		}
	delete[] _pinfos;
}
UINT16 ProcessGenerator::generate()
{
	// TODO : 할당할 수 없을 때 처리
	std::pair<int, PROCESS_INFORMATION*> val = this->getUsableInfo();
	if (val.second == nullptr)
		return 0;

	STARTUPINFOA si = {};

	si.cb = sizeof(STARTUPINFOA);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	std::string programArg = "C:\\Users\\taejeong\\source\\repos\\ServerFramework\\x64\\Debug\\GameServer.exe ";
	programArg += Engine::getEngineConfig((int)E_EngineType::MATCH_SERVER).ip + " ";
	programArg += std::to_string(val.first);
	std::cout << programArg << '\n';
	if (CreateProcessA("C:\\Users\\taejeong\\source\\repos\\ServerFramework\\x64\\Debug\\GameServer.exe", (LPSTR)programArg.c_str(),
		NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, val.second) == FALSE)
	{
		puts("CreateProcess error");
		return 0;
	}
	return val.first;
}
#pragma endregion

#pragma region private
std::pair<int, PROCESS_INFORMATION*> ProcessGenerator::getUsableInfo()
{
	for (int i = 0; i < ProcessGenerator::MAX_PORT_NUM - ProcessGenerator::MIN_PORT_NUM; ++i)
	{
		if (_pinfos[i].hProcess == INVALID_HANDLE_VALUE)
		{
			_pinfos[i] = {};
			return { ProcessGenerator::MIN_PORT_NUM + i, &_pinfos[i] };
		}
		if (WaitForSingleObject(_pinfos[i].hProcess, 0) == WAIT_OBJECT_0)
		{
			printf("simulation server (%d) is end\n", i);
			CloseHandle(_pinfos[i].hThread);
			CloseHandle(_pinfos[i].hProcess);
			_pinfos[i] = {};
			return { ProcessGenerator::MIN_PORT_NUM + i, &_pinfos[i] };
		}
	}
	return { 0, nullptr };
}
#pragma endregion