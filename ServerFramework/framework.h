#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <windows.h>
#include <malloc.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <memory>

// 오픈 소스 라이브러리
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

