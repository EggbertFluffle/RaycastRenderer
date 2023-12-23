#pragma once

namespace egg {
	struct vec2f {
		float x, y;
		vec2f(float _x, float _y):
			x(_x),
			y(_y) {};
	};

	struct vec2i {
		int x, y;
		vec2i(int _x, int _y):
			x(_x),
			y(_y) {};
	};
}
