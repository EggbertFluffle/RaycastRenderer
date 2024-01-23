#pragma once

#include <vector>
#include <string>
#include <ncurses.h>

#include "./Textures.h"

void swap(int& a, int& b);
void swapVec(int& x1, int& y1, int& x2, int& y2);
int fixStretch(int& y);
bool validPos(int& x, int& y, int& w, int& h);
void fillRect(int x1, int y1, int x2, int y2, char stroke, char fill);
void drawLine(int x1, int y1, int x2, int y2, char stroke);
void drawLine(int x1, int y1, int x2, int y2, char32_t stroke);
void drawTexturedLine(int& x, float& uTexCoord, int& fromCenter, int& width, int& height, texture* tex);
