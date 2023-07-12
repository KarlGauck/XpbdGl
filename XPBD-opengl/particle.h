#pragma once

#include "Vec2.h"

struct Particle {
	Particle(float mass, Vec2 pos, Vec2 vel);

	float mass;
	Vec2 oldPos;
	Vec2 pos;
	Vec2 vel;
};