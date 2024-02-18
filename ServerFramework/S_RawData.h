#pragma once

#include "Define.h"
// 암호화, 압축 처리가 되지 않은 데이터
struct S_RawData
{
	int to;
	Size blockSize;
	char* data; // 처리해야될 데이터 ( 헤더 + 데이터 )
};