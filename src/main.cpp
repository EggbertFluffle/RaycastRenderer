#include <clocale>
#include <vector>
#include <string>
#include <ncurses.h>

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
	const int mapWidth = 6, mapHeight = 6;
	const float scl = 8;
	const std::vector<char> shading = {'@', '#', 'W', 'N', '$', '?', 'a', '=', '-', '.'};
	// std::vector<int> map = {
	// 	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	// 	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	// 	1, 0, 1, 1, 1, 1, 1, 0, 0, 1,
	// 	1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
	// 	1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
	// 	1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
	// 	1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
	// 	1, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	// 	1, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	// 	1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	// };
	std::vector<int> map = {
		1, 2, 1, 2, 1, 2,
		2, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 2,
		2, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 2,
		2, 1, 2, 1, 2, 1
	};

	// In order to easily render the walls
	std::vector<egg::vec2i> walls;
	for(int y = 0; y < mapHeight; y++) {
		for(int x = 0; x < mapWidth; x++) {
			if(map[(y * mapWidth) + x] != 0) walls.push_back(egg::vec2i(x, y));
		}
	}

	Player player((mapWidth * scl) / 2 + 0.001, (mapHeight * scl) / 2 + 0.001, 0);
	bool quit = false;
	bool perspective = false;
	bool textured = true;

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

		eb::PushStack("player x", std::to_string(player.position.x));
		eb::PushStack("player y", std::to_string(player.position.y));
		eb::PushStack("angle", std::to_string(player.angle));

		int lines = width;
		std::vector<float> distances(lines);
		std::vector<int> hits(lines);
		player.TakePerspective(&distances, &hits, &map, &scl, &mapWidth, &mapHeight);
		// player.RayCast(&map, &scl, &mapWidth, &mapHeight, 0);

		if(perspective) {
			for(int i = 0; i < distances.size(); i++) {
				float fc = lerp((float) (height / 2.0f), 0.0f, (float) distances.at(i) < 30 ? distances.at(i) / 30 : 1);
				if(textured) {
					char c = (hits.at(i) == 1 ? '-' : '|');
					drawLine(i, height / 2.0f - fc, i, height / 2.0f + fc, c);
				} else {
					int c = (int) lerp(((float) shading.size()) - 1.0f, 0.0f, fc / ((float) height / 2));
					drawLine(i, height / 2.0f - fc, i, height / 2.0f + fc, shading[c]);
				}
			}

			// Draw the minimap
			fillRect(0, 0, mapWidth, mapHeight, ' ', ' ');
			for(egg::vec2i wall : walls) {
				mvaddch(wall.y, wall.x, '#');
				mvaddch((int) (player.position.y / scl), (int) (player.position.x / scl), 'P');
			}
		} else {
			// Draw all the walls
			for(egg::vec2i wall : walls) {
				fillRect(wall.x * scl, wall.y * scl, wall.x * scl + scl, wall.y * scl + scl, '#', '.');
				mvaddstr(wall.y * scl + 1, wall.x * scl + 1, (std::to_string(wall.x) + std::string(", ") + std::to_string(wall.y)).c_str());
			}

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
