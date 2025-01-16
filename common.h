#pragma once

struct Vector2
{
public:
	Vector2(float setx, float sety) {
		x = setx;
		y = sety;
	}
	Vector2() { x = 0; y = 0; }

	float x;
	float y;
};

enum state {
	error,
	move,
	bound,
	playerPoint,
	cpuPoint,
	preparation,
};