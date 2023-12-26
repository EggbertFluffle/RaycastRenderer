#include <vector>
#include <string>
#include <ncurses.h>

#include "./Eggmath.h"
#include "./Graphics.h"

#pragma once

namespace eb {
	void PushStack(const char* key, std::string val);
	void PrintStack(int x, int y);

	void PushLine(float x1, float y1, float x2, float y2, char c);
	void PrintLines();
}
