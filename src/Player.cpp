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

void Player::RayCast(std::vector<int>* map, const float* scl, const int* mapWidth, const int* mapHeight, float& _x, float& _y, float& dist, std::vector<std::string>* printStack) {
	// Unchanged
	float m = std::tan(angle);
	int renderDistance = 6;
	
	float hdx = (angle < PI * 0.5 || angle > PI * 1.5) ? 
		*scl - std::fmod(position.x, *scl) :
		-std::fmod(position.x, *scl);
	float hdy = m * hdx;
	float hx = hdx;
	float hy = hdy;
	int horizHit = 0;
	for(int i = 0; i < renderDistance && horizHit == 0; i++) {
		float inc = (hdx < 0 ? -*scl : *scl);
		hx = hdx + i * inc;
		hy = hdy + i * (m * inc);

		int mx = (int) ((position.x + hx) / *scl);
		int my = (int) ((position.y - hy) / *scl);

		int mapIndex = (my * *mapWidth) + (hdx < 0 ? mx - 1 : mx);
		if(mapIndex < map->size()) {
			horizHit = map->at(mapIndex);
		} else {
			horizHit = -1;
		}
	}

	float vdy = (angle > PI) ? 
		*scl - std::fmod(position.y, *scl) :
		-std::fmod(position.y, *scl);
	float vdx = vdy / m;
	float vx, vy;
	int vertHit = 0;
	for(int i = 0; i < renderDistance && vertHit == 0; i++) {
		float inc = (vdy < 0 ? -*scl : *scl);

		vy = vdy + i * inc;
		vx = vdx + i * (inc / m);

		int mx = (int) ((position.x - vx) / *scl);
		int my = (int) ((position.y + vy) / *scl);

		int mapIndex = ((vdy > 0 ? my : my - 1) * *mapWidth) + mx;
		if(mapIndex < map->size()) {
			vertHit = map->at(mapIndex);
		} else {
			vertHit = -1;
		}
	}

	float horizDistance = std::sqrt(std::pow(hx, 2) + std::pow(hy, 2));
	float vertDistance = std::sqrt(std::pow(vx, 2) + std::pow(vy, 2));
	if(horizDistance < vertDistance) {
		_x = position.x + hx;
		_y = position.y - hy;
		dist = horizDistance;
	} else {
		_x = position.x - vx;
		_y = position.y + vy;
		dist = vertDistance;
	}
}
