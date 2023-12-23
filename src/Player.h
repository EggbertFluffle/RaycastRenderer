#include <vector>
#include "./Eggmath.h"

#pragma once

class Player {
public:
	egg::vec2f position;
	float speed;
	float angle;
	
	Player(float _x, float _y, float _a);
	void HandleInput(char c);
	void WorldMove(float _x, float _y);
	void LocalMove(float _x, float _y);
	void RayCast(std::vector<int>* map, const float* scl, const int* mapWidth, const int* mapHeight, float& _x, float& _y, float& dist, std::vector<std::string>* printStack);
};
