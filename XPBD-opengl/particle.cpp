#include "Vec2.h"

#include "particle.h"

Particle::Particle(float mass, Vec2 pos, Vec2 vel): mass(mass), pos(pos), vel(vel), oldPos(pos)
{}