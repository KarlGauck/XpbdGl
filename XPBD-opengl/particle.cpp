#include "Vec2.h"

#include "particle.h"

Particle::Particle(float mass, float radius, Vec2 pos, Vec2 vel) : mass(mass), radius(radius), pos(pos), vel(vel), oldPos(pos)
{}