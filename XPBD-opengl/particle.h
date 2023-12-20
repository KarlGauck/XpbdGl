#pragma once

#include "Vec2.h"

struct Particle {
	Particle(float mass, float radius, Vec2 pos, Vec2 vel, float color[], bool fluid);

	float mass;
	float density;
	float radius;
	Vec2 oldPos;
	Vec2 pos;
	Vec2 vel;
	float color[4];
	bool fluid;
};