#include <vector>
#include <string>
#include <ncurses.h>

#include "./Graphics.h"
#include "./Eggmath.h"
#include "./Player.h"

#define PI 3.1415926

void PrintStack(std::vector<std::string> stack, int x, int y);

int main () {
	int width, height;


	// Initialize ncurses
	initscr();
	getmaxyx(stdscr, height, width);
	keypad(stdscr, TRUE);
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	mouseinterval(0);


	// Map information
	const int mapWidth = 6, mapHeight = 6;
	const float scl = 8;
	/* std::vector<int> map = {
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
	}; */
	std::vector<int> map = {
		1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1
	};

	// In order to easily render the walls
	std::vector<egg::vec2i> walls;
	for(int y = 0; y < mapHeight; y++) {
		for(int x = 0; x < mapWidth; x++) {
			if(map[(y * mapWidth) + x] == 1) walls.push_back(egg::vec2i(x, y));
		}
	}

	Player player((mapWidth * scl) / 2, (mapHeight * scl) / 2, 0);
	bool quit = false;

	while(!quit) {
		int c = wgetch(stdscr);

		// Hold all the values to be printed for "debugging" i wanna kms
		std::vector<std::string> printStack;

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
		}

		// Clean previous pixel buffer
		erase();

		// Draw all the walls
		for(egg::vec2i wall : walls) {
			fillRect(wall.x * scl, wall.y * scl, wall.x * scl + scl, wall.y * scl + scl, '#', '.');
		}

		// Debug coordinates
		for(egg::vec2i wall : walls) {
			mvaddstr(wall.y * scl + 1, wall.x * scl + 1, (std::to_string(wall.x) + std::string(", ") + std::to_string(wall.y)).c_str());
		}

		// nx = final x of ray, same with y, and dist is distance of the ray
		float nx, ny, dist;
		player.RayCast(&map, &scl, &mapWidth, &mapHeight, nx, ny, dist, &printStack);
		drawLine(player.position.x, player.position.y, nx, ny, '@');
		mvaddch(player.position.y, player.position.x, 'P');

		printStack.push_back(std::string("angle: ") + std::to_string(player.angle));
		printStack.push_back(std::string("nx: ") + std::to_string(nx));
		printStack.push_back(std::string("ny: ") + std::to_string(ny));
		printStack.push_back(std::string("Ray dist: ") + std::to_string(dist));

		PrintStack(printStack, mapWidth * scl + scl, 1);

		refresh();
	}

	endwin();
	return 0;
}

void PrintStack(std::vector<std::string> stack, int x, int y) {
	for(int i = 0; i < stack.size(); i++) {
		std::string* str = &stack[i];
		mvaddstr(y + (i * 2), x, str->c_str());
	}
}


// Mouse position code

