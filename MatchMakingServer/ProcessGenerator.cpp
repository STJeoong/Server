#include "ProcessGenerator.h"
#include "Server.h"
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

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
	// TODO : �Ҵ��� �� ���� �� ó��
	std::pair<int, PROCESS_INFORMATION*> val = this->getUsableInfo();
	if (val.second == nullptr)
		return 0;

	HANDLE hPipe;

	hPipe = CreateNamedPipe(
        _T("\\\\.\\pipe\\SimulationPipe"),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        4096, 4096, 0, NULL);
	if (hPipe == INVALID_HANDLE_VALUE)
		return 0;

	STARTUPINFOA si = {};

	si.cb = sizeof(STARTUPINFOA);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	std::string programArg = "C:\\Users\\taejeong\\Desktop\\SimulationServer\\SimulationServer.exe ";
	programArg += Server::getInstance().getServerConfig().ip + " ";
	programArg += std::to_string(val.first);
	std::cout << programArg << '\n';
	if (CreateProcessA("C:\\Users\\taejeong\\Desktop\\SimulationServer\\SimulationServer.exe", (LPSTR)programArg.c_str(),
		NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, val.second) == FALSE)
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
			CloseHandle(_pinfos[i].hThread);
			CloseHandle(_pinfos[i].hProcess);
			_pinfos[i] = {};
			return { ProcessGenerator::MIN_PORT_NUM + i, &_pinfos[i] };
		}
	}
	return { 0, nullptr };
}
#pragma endregion