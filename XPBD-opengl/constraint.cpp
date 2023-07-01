#include <vector>
#include <iostream>

#include "constraint.h"
#include "particle.h"

Constraint::Constraint(std::vector<Particle*> particles, float compliance): compliance(compliance)
{
	this->particles = particles;
}

void Constraint::calculateLambda(float dt)
{
	float weightedMass = 0.f;
	for (Particle* particlePtr : particles)
	{
		Particle& particle = *particlePtr;
		Vec2 grad = gradient(particle);
		weightedMass += (1/particle.mass) * grad.dot(grad);
	}
	lambda = -error / (weightedMass + (compliance/pow(dt, 2)));
}

Vec2 Constraint::deltaPos(Particle& particle)
{
	Vec2 grad = gradient(particle);
	return grad * lambda * (1/particle.mass);
}