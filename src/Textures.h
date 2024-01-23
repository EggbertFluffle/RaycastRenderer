#include <unordered_map>
#include <vector>

#pragma once

struct texture {
	unsigned short size;
	bool hasTexture;
	std::vector<char> tex;

	texture();
	texture(unsigned short s, const char* m);
};
