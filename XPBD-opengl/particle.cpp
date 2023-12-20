#include "Vec2.h"

#include "particle.h"

Particle::Particle(float mass, float radius, Vec2 pos, Vec2 vel, float color[], bool fluid) : mass(mass), radius(radius), pos(pos), vel(vel), oldPos(pos), fluid(fluid)
{
	for (int i = 0; i < 4; i++)
		this->color[i] = color[i];
}

// (or (= x 7) (not (= y 7)))