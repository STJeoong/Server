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
	// TODO : �Ҵ��� �� ���� �� ó��
	std::pair<int, PROCESS_INFORMATION*> val = this->getUsableInfo();
	if (val.second == nullptr)
		return 0;

	SECURITY_ATTRIBUTES sa = {}; // ���� �Ӽ�
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE; // �ڵ� ��� ����
	sa.lpSecurityDescriptor = NULL;

	HANDLE hPipeWrite;
	HANDLE hPipeRead;
	if (CreatePipe(&hPipeRead, &hPipeWrite, &sa, 0) == FALSE) // ������ ����
	{
		puts("CreatePipe error");
		return 1;
	}

	STARTUPINFO si = {}; // �ڽ� ���μ��� �Ӽ�

	si.cb = sizeof(STARTUPINFO);
	si.hStdOutput = hPipeWrite; // �����̷���
	si.hStdError = hPipeWrite; // �����̷���
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES; // STARTF_USESTDHANDLES �ɼ��� �ɾ���� si.hStdOutput ���� �־��ذ� ����ȴ�.
	si.wShowWindow = SW_NORMAL;

	std::wstring programArg = _T("C:\\Users\\taejeong\\Desktop\\SimulationServer\\SimulationServer.exe ");
	programArg += std::to_wstring(val.first);
	if (CreateProcess(_T("C:\\Users\\taejeong\\Desktop\\SimulationServer\\SimulationServer.exe"), (LPWSTR)programArg.c_str(),
		NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, val.second) == FALSE)
	{
		puts("CreateProcess error");
		return 1;
	}
	char buf[1024];
	DWORD readByte = 0;
	if (ReadFile(hPipeRead, buf, 1024, &readByte, NULL) == TRUE)
	{
		buf[readByte] = '\0';
		printf("from simulation server : %s\n", buf);
	}
	else
		puts("ReadFile error");

	CloseHandle(hPipeRead);
	CloseHandle(hPipeWrite);
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