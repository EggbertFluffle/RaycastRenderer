#include <vector>

#include "./Textures.h"

texture::texture() :
	hasTexture(false)
{};

texture::texture(unsigned short s, const char* m) :
	hasTexture(true),
	size(s),
	tex(std::vector<char>(s * s))
{
	for(int i = 0; i < s * s; i++) {
		tex[i] = *(m + i);
	}
}
