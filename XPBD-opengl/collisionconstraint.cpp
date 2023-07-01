#include <vector>

#include "constraint.h"
#include "collisionconstraint.h"
#include "particle.h"

CollisionConstraint::CollisionConstraint(std::vector<Particle*> particles) : Constraint(particles, 0.f)
{};

Vec2 CollisionConstraint::gradient(Particle& particle)
{
	Particle* other = nullptr;
	if (particles[0] == &particle)
		other = particles[1];
	else other = particles[0];
	return (other->pos - particle.pos).normalized();
}

void CollisionConstraint::calculateError()
{
	float distance = particles[0]->pos.distance(particles[1]->pos);
	if (distance < 1)
		error = 1-distance;
	else error = 0;
}