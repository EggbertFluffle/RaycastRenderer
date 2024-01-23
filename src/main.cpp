#include <clocale>
#include <vector>
#include <string>
#include <ncurses.h>
#include <iostream>

#include "./Graphics.h"
#include "./Eggmath.h"
#include "./Player.h"
#include "./Eggbug.h"
#include "./Textures.h"

#define PI 3.1415926

float lerp(float a, float b, float f);

int main () {
	int width, height;

	// Initialize ncurses
	setlocale(LC_ALL, "");
	initscr();
	getmaxyx(stdscr, height, width);
	keypad(stdscr, TRUE);
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	std::cout << "\033[?1003h\n";
	mouseinterval(0);


	// Map information
	const int mapWidth = 6, mapHeight = 6;
	const float scl = 6;
	const std::vector<char> shading = {'@', '#', 'W', '$', '?', 'a', '(', '>', '=', '-', '.'};
	// std::vector<int> map = {
	// 	1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
	// 	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	// 	2, 0, 2, 1, 2, 1, 2, 0, 0, 2,
	// 	1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
	// 	2, 0, 2, 0, 0, 0, 0, 0, 0, 2,
	// 	1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
	// 	2, 0, 2, 0, 0, 0, 0, 0, 0, 2,
	// 	1, 0, 0, 0, 0, 0, 0, 2, 1, 1,
	// 	2, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	// 	1, 1, 2, 1, 2, 1, 2, 1, 0, 0
	// };
	std::vector<int> map = {
		1, 2, 1, 2, 1, 2,
		2, 0, 0, 0, 0, 1,
		1, 0, 3, 0, 0, 2,
		2, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 2,
		2, 1, 2, 1, 2, 1
	};

	std::vector<texture> textures;
	textures.push_back(texture());
	textures.push_back(texture(8, 
				"########"
				"########"
				"########"
				"########"
				"########"
				"########"
				"########"
				"########"));
	textures.push_back(texture(8, 
				"########"
				"########"
				"# #    #"
				"#  #   #"
				"#   #  #"
				"#    # #"
				"########"
				"########"));
	textures.push_back(texture(16, 
				"################"
				"#              #"
				"#              #"
				"#       #      #"
				"#       ###    #"
				"#       ####   #"
				"#       #####  #"
				"# ############ #"
				"# ############ #"
				"#       #####  #"
				"#       ####   #"
				"#       ###    #"
				"#       ##     #"
				"#              #"
				"#              #"
				"################"));

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
	short textured = 0;

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
			case 'a':
				player.HandleInput(c);
				break;
			case 'd':
				player.HandleInput(c);
				break;
			case 'j':
				player.HandleInput(c);
				break;
			case 'k':
				player.HandleInput(c);
				break;
			case 'm':
				player.HandleInput(c);
				break;
			case KEY_MOUSE:
				player.HandleInput('M');
				break;
			case 'v':
				perspective = !perspective; 
				break;
			case 't':
				textured++;
				textured %= 3;
				break;
		}

		// Clean previous pixel buffer
		erase();

		int lines = width;
		std::vector<float> distances(lines);
		std::vector<int> hits(lines);
		std::vector<float> uTexCoords(lines);
		player.TakePerspective(&distances, &hits, &uTexCoords, &map, &scl, &mapWidth, &mapHeight);

		if(perspective) {
			// PERSPECTIVE VIEW
			for(int i = 0; i < distances.size(); i++) {
				// float fc = lerp((float) (height / 2.0f), 0.0f, (float) distances.at(i) < 30 ? distances.at(i) / 30 : 1);
				int fromCenter = (height / float(distances.at(i))) * 1.5;
				if(textured == 0) {
					char c = (hits.at(i) == 1 ? '-' : '|');
					drawLine(i, height / 2.0f - fromCenter, i, height / 2.0f + fromCenter, c);
				} else if(textured == 1) {
					int c = (int) lerp(((float) shading.size()) - 1.0f, 0.0f, fromCenter / ((float) height / 2));
					drawLine(i, height / 2.0f - fromCenter, i, height / 2.0f + fromCenter, shading[c]);
				} else if(textured == 2) {
					drawTexturedLine(i, uTexCoords.at(i), fromCenter, width, height, &textures.at(hits.at(i)));
				}
			}

			// Draw the minimap
			fillRect(0, 0, mapWidth, mapHeight, ' ', ' ');
			for(egg::vec2i wall : walls) {
				mvaddch(wall.y, wall.x, '#');
				mvaddch((int) (player.position.y / scl), (int) (player.position.x / scl), 'P');
			}
		} else {
			// MAP VIEW
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
