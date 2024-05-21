#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <queue>
#include <stack>
#include <limits>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

// define
#define PI 3.14159265359f
#define MIN_SIZE 0.0001f // minimum size of circle, box
#define BAUMGART 0.2f // how fast you want to fix violation. 0~1. the bigger this value is, the faster it fixes violation.
#define COLLIDER_MARGIN 0.01f // this makes collider can have margin space.