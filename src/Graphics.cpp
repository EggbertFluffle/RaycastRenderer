#include <string>
#include <vector>
#include <ncurses.h>
#include "Graphics.h"

void swap(int& a, int& b) {
	int t = b;
	b = a;
	a = t;
}

void swapVec(int& x1, int& y1, int& x2, int& y2) {
	int tx = x2;
	x2 = x1;
	x1 = tx;

	int ty = y2;
	y2 = y1;
	y1 = ty;
}

int fixStretch(int y) {
	return (int) (((float) y) * 0.66);
}

bool validPos(int& x, int& y, int& w, int& h) {
	if( x < 0 ||
		x > w || 
		y < 0 ||
		y > h
	) {	return false; }
	return true;
}

void drawLine(int x1, int y1, int x2, int y2, char stroke) {
	if(x1 > x2 || y1 > y2) {
		swapVec(x1, y1, x2, y2);
	}

	float dx = x2 - x1;
	float dy = y2 - y1;
	float m = dy / dx;
	float b = -m * x1 + y1;

	if(dx == 0) {
		for(int y = y1; y < y2 + 1; y += 1) {
			mvaddch(y, x1, stroke);
		}	
	}

	if(m <= 1 && m >= -1) {
		for(int x = x1; x < x2; x += 1) {
			int ry = (int) (m * x + b) + 0.5;
			mvaddch(ry, x, stroke);
		}
	} else {
		for(int y = y1; y < y2; y += 1) {
			int rx = (int) ((y - b) / m) + 0.5;
			mvaddch(y, rx, stroke);
		}
	}
}

void drawLine(int x1, int y1, int x2, int y2, char stroke, std::vector<std::string>* printStack) {
	if(x1 > x2) {
		swapVec(x1, y1, x2, y2);
	}

	float dx = x2 - x1;
	float dy = y2 - y1;
	float m = dy / dx;
	float b = -m * x1 + y1;

	if(dx == 0) {
		for(int y = y1; y < y2 + 1; y += 1) {
			mvaddch(y, x1, stroke);
		}	
	}

	if(m <= 1 && m >= -1) {
		for(int x = x1; x < x2; x += 1) {
			int ry = (int) (m * x + b) + 0.5;
			mvaddch(ry, x, stroke);
		}
	} else {
		for(int y = y1; y < y2; y += 1) {
			int rx = (int) ((y - b) / m) + 0.5;
			mvaddch(y, rx, stroke);
		}
	}

	printStack->push_back(std::string("slope: ") + std::to_string(m));
	printStack->push_back(std::string("dx: ") + std::to_string(dx));
	printStack->push_back(std::string("dy: ") + std::to_string(dy));
	printStack->push_back(std::string("b: ") + std::to_string(b));

	mvaddch(y1, x1, 'X');
	mvaddch(y2, x2, 'X');
	mvaddstr(y1 + 2, x1 + 2, (std::to_string(x1) + std::string(", ") + std::to_string(y1)).c_str());
	mvaddstr(y2 + 2, x2 + 2, (std::to_string(x2) + std::string(", ") + std::to_string(y2)).c_str());
}

void fillRect(int x1, int y1, int x2, int y2, char stroke, char fill) {
	if(x1 > x2 || y1 > y2) {
		swapVec(x1, y1, x2, y2);
	}

	for(int x = x1 + 1; x < x2; x++) {
		for(int y = y1 + 1; y < y2; y++) {
			mvaddch(x, y, fill);
		}
	}

	drawLine(x1, y1, x2, y1, stroke);
	drawLine(x2, y1, x2, y2, stroke);
	drawLine(x2, y2, x1, y2, stroke);
	drawLine(x1, y2, x1, y1, stroke);
}
