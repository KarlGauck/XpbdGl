#include <vector>
#include <iostream>

#include "distanceconstraint.h"
#include "Vec2.h"

DistanceConstraint::DistanceConstraint(std::vector<Particle*> particles, float compliance, float distance): Constraint(particles, compliance)
{
	this->distance = distance;
	if (particles.size() != 2) {
		std::cout << "Particle vector of distance constraint must be of size 2" << std::endl;
	}
};

Vec2 DistanceConstraint::gradient(Particle& particle)
{
	Particle* other = nullptr;
	if (particles[0] == &particle)
		other = particles[1];
	else other = particles[0];
	return (particle.pos - other->pos).normalized();
}

void DistanceConstraint::calculateError()
{
	float dist = (*particles[0]).pos.distance((*particles[1]).pos);
	error = (*particles[0]).pos.distance((*particles[1]).pos) - distance;
}