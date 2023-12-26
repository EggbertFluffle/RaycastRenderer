#include <clocale>
#include <vector>
#include <string>
#include <ncurses.h>
#include <cmath>

#include "./Graphics.h"
#include "./Eggmath.h"
#include "./Player.h"
#include "./Eggbug.h"

#define PI 3.1415926

float lerp(float a, float b, float f);

int main () {
	int width, height;

	// Initialize ncurses
	setlocale(LC_ALL, "");
	initscr();
	getmaxyx(stdscr, height, width);
	keypad(stdscr, TRUE);
	// mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	// mouseinterval(0);


	// Map information
	const int mapWidth = 10, mapHeight = 10;
	const float scl = 8;
	const char shading[] = "@@##WW$$?!a:=-,. ";
	std::vector<int> map = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 1, 1, 1, 1, 1, 0, 0, 1,
		1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};
	// std::vector<int> map = {
	// 	1, 1, 1, 1, 1, 1,
	// 	1, 0, 0, 0, 0, 1,
	// 	1, 0, 0, 0, 0, 1,
	// 	1, 0, 0, 0, 0, 1,
	// 	1, 0, 0, 0, 0, 1,
	// 	1, 1, 1, 1, 1, 1
	// };

	// In order to easily render the walls
	std::vector<egg::vec2i> walls;
	for(int y = 0; y < mapHeight; y++) {
		for(int x = 0; x < mapWidth; x++) {
			if(map[(y * mapWidth) + x] == 1) walls.push_back(egg::vec2i(x, y));
		}
	}

	Player player((mapWidth * scl) / 2, (mapHeight * scl) / 2, 0);
	bool quit = false;
	bool perspective = false;

	while(!quit) {
		int c = wgetch(stdscr);

		switch(c) {
			case 'Q':
				quit = true;
				break;
			case 'w':
				player.HandleInput(c);
				break;
			case 's':
				player.HandleInput(c);
				break;
			case 'j':
				player.HandleInput(c);
				break;
			case 'k':
				player.HandleInput(c);
				break;
			case 'v':
				perspective = !perspective; 
				break;
		}

		// Clean previous pixel buffer
		erase();

		int lines = width;
		std::vector<float> distances(lines);
		player.TakePerspective(&distances, &map, &scl, &mapWidth, &mapHeight);
		// player.RayCast(&map, &scl, &mapWidth, &mapHeight, 0);

		if(perspective) {
			for(int i = 0; i < distances.size(); i++) {
				float fc = lerp((float) height / 2.0, 0.0, (float) distances.at(i) < 30 ? distances.at(i) / 30 : 0);
				char c = shading[(int) lerp(sizeof(shading)/sizeof(char), 0, fc / 30.0)];
				eb::PushStack("Letter", std::to_string(c));
				drawLine(i, height / 2.0 - fc, i, height / 2.0 + fc, c);
			}
		} else {
			// Draw all the walls
			for(egg::vec2i wall : walls) {
				fillRect(wall.x * scl, wall.y * scl, wall.x * scl + scl, wall.y * scl + scl, '#', '.');
				mvaddstr(wall.y * scl + 1, wall.x * scl + 1, (std::to_string(wall.x) + std::string(", ") + std::to_string(wall.y)).c_str());
			}

			eb::PushStack("angle", std::to_string(player.angle));

			eb::PrintStack(mapWidth * scl + scl, 1);
			eb::PrintLines();

			mvaddch(player.position.y, player.position.x, 'P');
		}
		// Ensure the cursor is out of the way
		mvaddch(width, height, '~');
		refresh();
	}

	endwin();
	return 0;
}

float lerp(float a, float b, float f) {
    return a + f * (b - a);
};
