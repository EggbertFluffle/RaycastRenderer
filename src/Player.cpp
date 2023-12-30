#include <cmath>
#include <ncurses.h>
#include <random>
#include <vector>

#include "./Eggmath.h"
#include "./Player.h"
#include "./Eggbug.h"

#define PI 3.1415926

float lerp(float a, float b, float t);

Player::Player(float _x, float _y, float _a):
		position(_x, _y),
		angle(_a),
		speed(0.5),
		fov(PI * 0.2)
{};

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
			if (angle < 0) angle = 2 * PI + angle;
			break;
	}
}

float Player::RayCast(int&hit, std::vector<int>* map, const float* scl, const int* mapWidth, const int* mapHeight, float angleDiff) {
	// Constants between horizontal and vertical
	float rayAngle = (angle + angleDiff) < 0 ? 2 * PI + (angle + angleDiff) : std::fmod(angle + angleDiff, 2 * PI);
	float m = -std::tan(rayAngle);
	int renderDistance = 14;
	
	// Horizontal line checks
	float hdx = (rayAngle < PI * 0.5 || rayAngle > PI * 1.5) ? 
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
		int my = (int) ((position.y + hy) / *scl);

		int mapIndex = (my * *mapWidth) + (hdx < 0 ? mx - 1 : mx);
		if(mapIndex < map->size()) {
			horizHit = map->at(mapIndex);
		} else {
			horizHit = -1;
		}
	}

	// Vertical line checks
	float vdy = (rayAngle > PI) ? 
		*scl - std::fmod(position.y, *scl) :
		-std::fmod(position.y, *scl);
	float vdx = vdy / m;
	float vx, vy;
	int vertHit = 0; for(int i = 0; i < renderDistance && vertHit == 0; i++) {
		float inc = (vdy < 0 ? -*scl : *scl);

		vy = vdy + i * inc;
		vx = vdx + i * (inc / m);

		int mx = (int) ((position.x + vx) / *scl);
		int my = (int) ((position.y + vy) / *scl);

		int mapIndex = ((vdy > 0 ? my : my - 1) * *mapWidth) + mx;
		if(mapIndex < map->size()) {
			vertHit = map->at(mapIndex);
		} else {
			vertHit = -1;
		}
	}

	// Pick shortest distance
	float horizDistance = std::sqrt(std::pow(hx, 2) + std::pow(hy, 2));
	float vertDistance = std::sqrt(std::pow(vx, 2) + std::pow(vy, 2));
	if(horizDistance < vertDistance) {
		eb::PushLine(position.x, position.y, position.x + hx, position.y + hy, 'H');
		hit = horizHit;
		return horizDistance * std::cos(angleDiff);
	} else {
		eb::PushLine(position.x, position.y, position.x + vx, position.y + vy, 'V');
		hit = vertHit;
		return vertDistance * std::cos(angleDiff);
	}
	// return std::min(horizDistance, vertDistance) * std::cos(angleDiff);
}

void Player::TakePerspective(std::vector<float>* distances, std::vector<int>* hits,
		std::vector<int>* map, const float* scl, const int* mapWidth, const int* mapHeight)
{
	for(int i = 0; i < distances->size(); i++) {
		distances->at(i) = RayCast(
				hits->at(i),
				map, scl, mapWidth, mapHeight,
				lerp(PI * 0.25, PI * -0.25, (float) i / (float) distances->size()));
	}
}

