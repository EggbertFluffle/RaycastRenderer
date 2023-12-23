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

		erase();

		for(egg::vec2i wall : walls) {
			fillRect(wall.x * scl, wall.y * scl, wall.x * scl + scl, wall.y * scl + scl, '#', '.');
		}

		std::vector<std::string> printStack;
		float nx, ny, dist;
		player.RayCast(map, &scl, &mapWidth, &mapHeight, nx, ny, dist);
		drawLine(player.position.x, player.position.y, nx, ny, '@');
		drawLine(25, 35, 55, 25, '0', &printStack);
		mvaddch(player.position.y, player.position.x, 'P');

		printStack.push_back(std::string("angle: ") + std::to_string(player.angle));
		printStack.push_back(std::string("Ray X: ") + std::to_string(nx));
		printStack.push_back(std::string("Ray Y: ") + std::to_string(ny));
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

/* if(c == KEY_MOUSE) {
	MEVENT mevent;
	if(getmouse(&mevent) == OK) {
		pos.x = mevent.x;
		pos.y = mevent.y;
	}
} */
