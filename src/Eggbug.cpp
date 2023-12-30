#include <vector>
#include <string>

#include "./Eggmath.h"
#include "./Eggbug.h"

namespace eb {
	std::vector<std::string> printStack;

	void PushStack(const char* key, std::string val) {
		eb::printStack.push_back(std::string(key) + std::string(": ") + val);
	}

	void SeperateStack() {
		eb::printStack.push_back(std::string("-------------------------------"));
	}

	void PrintStack(int x, int y) {
		for(int i = 0; i < eb::printStack.size(); i++) {
			std::string* str = &eb::printStack[i];
			mvaddstr(y + (i * 2), x, str->c_str());
		}
		printStack.clear();
	}

	std::vector<egg::linef> lineStack;
	std::vector<char> lineStackStrokes;

	void PushLine(float x1, float y1, float x2, float y2, char c) {
		lineStack.push_back(egg::linef(x1, y1, x2, y2));
		lineStackStrokes.push_back(c);
	}

	void PrintLines() {
		for(int i = 0; i < eb::lineStack.size(); i++) {
			egg::linef* line = &eb::lineStack[i];
			drawLine(line->p1.x, line->p1.y, line->p2.x, line->p2.y, eb::lineStackStrokes.at(i));
		}
		lineStack.clear();
		lineStackStrokes.clear();
	}
}
