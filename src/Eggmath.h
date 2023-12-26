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

	struct linef {
		egg::vec2f p1, p2;

		linef(float x1, float y1, float x2, float y2): 
			p1(egg::vec2f(x1, y1)),
			p2(egg::vec2f(x2, y2)) {};

		linef(egg::vec2f _p1, egg::vec2f _p2):
			p1(_p1),
			p2(_p2) {};
	};
}
