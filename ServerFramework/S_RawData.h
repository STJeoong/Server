#pragma once

#include "Define.h"
// ��ȣȭ, ���� ó���� ���� ���� ������
struct S_RawData
{
	int to;
	Size blockSize;
	char* data; // ó���ؾߵ� ������ ( ��� + ������ )
};