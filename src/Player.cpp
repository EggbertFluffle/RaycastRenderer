#include <cmath>
#include <ncurses.h>
#include <string>
#include <vector>
#include "./Eggmath.h"
#include "./Player.h"

#define PI 3.1415926

Player::Player(float _x, float _y, float _a):
		position(_x, _y),
		angle(_a),
		speed(0.5) {};

void Player::WorldMove(float _x, float _y) {
	position.x += _x;
	position.y += _y;
}

void Player::LocalMove(float _x, float _y) {
	WorldMove(std::cos(angle) * _y, -std::sin(angle) * _y);
}

void Player::HandleInput(char c) {
	switch(c) {
		case 'w':
			LocalMove(0, speed);
			break;
		case 's':
			LocalMove(0, -speed);
			break;
		case 'j':
			angle += 0.1;
			if(angle > 2 * PI) angle = std::fmod(angle, (2 * PI));
			break;
		case 'k':
			angle -= 0.1;
			if (angle < 0) angle = 2 * PI  + angle;
			break;
	}
}

void Player::RayCast(std::vector<int> map, const float* scl, const int* mapWidth, const int* mapHeight, float& _x, float& _y, float& dist) {
	float m = std::tan(angle);
	float dx = (angle < PI * 0.5 || angle > PI * 1.5) ? 
		*scl - std::fmod(position.x, *scl) :
		-std::fmod(position.x, *scl);
	float dy = m * dx;

	float hx = dx;
	float hy = dy;

	int hit = 0;
	for(int i = 0; i < 6 && hit == 0; i++) {
		hx += (dx < 0 ? -*scl : *scl);
		hy += m * (dx < 0 ? -*scl : *scl);

		int mx = (int) ((position.x + hx) / *scl);
		int my = (int) ((position.y + hy) / *scl);

		mvaddstr(13 , *mapWidth * *scl + *scl, std::to_string(mx).c_str());
		mvaddstr(15 , *mapWidth * *scl + *scl, std::to_string(my).c_str());

		int mapIndex = (my * *mapWidth) + mx;
		if(mapIndex < (*mapHeight * *mapWidth)) {
			hit = map[mapIndex];
		} else {
			hit = -1;
		}
	}

	_x = position.x + hx;
	_y = position.y - hy;
	float horizDistance = std::sqrt(std::pow(hx, 2) + std::pow(hy, 2));
	dist = horizDistance;
}
