#include "ProcessGenerator.h"
#include "Server.h"
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

#pragma region public
ProcessGenerator::ProcessGenerator()
{
	_pinfos = new PROCESS_INFORMATION[ProcessGenerator::MAX_PORT_NUM - ProcessGenerator::MIN_PORT_NUM];
	for (int i = 0; i < ProcessGenerator::MAX_PORT_NUM - ProcessGenerator::MIN_PORT_NUM; ++i)
		_pinfos->hProcess = INVALID_HANDLE_VALUE;
}
ProcessGenerator::~ProcessGenerator() { delete[] _pinfos; }
UINT16 ProcessGenerator::generate()
{
	// TODO : 할당할 수 없을 때 처리
	std::pair<int, PROCESS_INFORMATION*> val = this->getUsableInfo();
	if (val.second == nullptr)
		return 0;

	HANDLE hPipe;
	char buffer[100];
	DWORD bytesRead;

	hPipe = CreateNamedPipe(
        _T("\\\\.\\pipe\\SimulationPipe"),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        4096, 4096, 0, NULL);
	if (hPipe == INVALID_HANDLE_VALUE)
		return 0;

	STARTUPINFO si = {};

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_NORMAL;

	std::wstring programArg = _T("C:\\Users\\taejeong\\Desktop\\SimulationServer\\SimulationServer.exe ");
	programArg += std::to_wstring(val.first);
	if (CreateProcess(_T("C:\\Users\\taejeong\\Desktop\\SimulationServer\\SimulationServer.exe"), (LPWSTR)programArg.c_str(),
		NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, val.second) == FALSE)
	{
		puts("CreateProcess error");
		CloseHandle(hPipe);
		return 0;
	}

	if (!ConnectNamedPipe(hPipe, NULL)) {
		puts("pipe connect error");
		CloseHandle(hPipe);
		return 0;
	}
	ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, NULL);
	buffer[bytesRead] = '\0';
	printf("from simulation server: %s\n", buffer);
	CloseHandle(hPipe);
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
			CloseHandle(_pinfos[i].hProcess);
			CloseHandle(_pinfos[i].hThread);
			_pinfos[i] = {};
			return { ProcessGenerator::MIN_PORT_NUM + i, &_pinfos[i] };
		}
	}
	return { 0, nullptr };
}
#pragma endregion