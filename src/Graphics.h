#pragma once

#include <vector>
#include <string>
#include <ncurses.h>

void swap(int& a, int& b);
void swapVec(int& x1, int& y1, int& x2, int& y2);
int fixStretch(int& y);
bool validPos(int& x, int& y, int& w, int& h);
void fillRect(int x1, int y1, int x2, int y2, char stroke, char fill);
void drawLine(int x1, int y1, int x2, int y2, char stroke);
