#pragma once
#include <nlohmann/json.hpp>

struct S_RectDefine
{
	S_RectDefine() = default;
	S_RectDefine(int offY, int offX, int yEx, int xEx) : offsetY(offY), offsetX(offX), yExtension(yEx), xExtension(xEx) {}
	int offsetY;
	int offsetX;
	int yExtension;
	int xExtension;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_RectDefine, offsetY, offsetX, yExtension, xExtension)