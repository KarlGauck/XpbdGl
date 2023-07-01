#include "Vec2.h"

#include "particle.h"

Particle::Particle(float mass, Vec2 pos, Vec2 vel) 
{
	this->mass = mass;
	this->pos = pos;
	this->vel = vel;
}