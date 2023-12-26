#include <vector>
#include <string>
#include "./Eggmath.h"

#pragma once

class Player {
public:
	egg::vec2f position;
	float speed;
	float angle;
	float fov;
	
	Player(float _x, float _y, float _a);
	void HandleInput(char c);
	void WorldMove(float _x, float _y);
	void LocalMove(float _x, float _y);
	float RayCast(std::vector<int>* map, const float* scl, const int* mapWidth, const int* mapHeight, float angleDiff);
	void TakePerspective(
			std::vector<float>* distances,
			std::vector<int>* map, const float* scl, const int* mapWidth, const int* mapHeight);
};
