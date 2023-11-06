#include <vector>
#include <iostream>

#include "distanceconstraint.h"
#include "Vec2.h"

int DistanceConstraint::currentIndex = 0;
DistanceConstraint DistanceConstraint::constraints[ConstraintCountMax];

DistanceConstraint::DistanceConstraint() {};

DistanceConstraint::DistanceConstraint(int particles[ParticleCountMax], float distance):
	distance(distance), compliance(0.0f)
{
	for (int i = 0; i < ParticleCountMax; i++)
	{
		this->particles[i] = particles[i];
	}
};

void DistanceConstraint::add(int particles[ParticleCountMax], float distance)
{
	if (currentIndex >= ConstraintCountMax)
		return;
	constraints[currentIndex] = DistanceConstraint(particles, distance);
	currentIndex++;
}

void DistanceConstraint::solve(std::vector<Particle>& globalParticles, float dt)
{
	for (int constraintIndex = 0; constraintIndex < currentIndex; constraintIndex++)
	{
		// Calculate Gradients
		DistanceConstraint& constraint = constraints[constraintIndex];
		for (int particleIndex = 0; particleIndex < ParticleCountMax; particleIndex++)
			constraint.gradient[particleIndex] = constraint.calculateGradient(globalParticles, globalParticles[constraint.particles[particleIndex]]);

		// Calculate Error
		constraint.calculateError(globalParticles);

		// Calculate Lamba
		float weightedMass = 0.f;
		for (int particleIndex = 0; particleIndex < ParticleCountMax; particleIndex++)
		{
			Particle& particle = globalParticles[constraint.particles[particleIndex]];
			Vec2 grad = constraint.gradient[particleIndex];
			weightedMass += (1/particle.mass) * grad.dot(grad);
		}
		constraint.lambda = -constraint.error / (weightedMass + (constraint.compliance/pow(dt, 2)));

		for (int particleIndex = 0; particleIndex < ParticleCountMax; particleIndex++)
		{
			Particle& particle = globalParticles[constraint.particles[particleIndex]];
			Vec2 deltaPos = constraint.gradient[particleIndex] * constraint.lambda * (1/particle.mass);
			particle.oldPos = particle.pos;
			particle.pos += deltaPos;
		}
	}

}

Vec2 DistanceConstraint::calculateGradient(std::vector<Particle>& globalParticles, Particle& particle)
{
	Particle* other = nullptr;
	if (&globalParticles[particles[0]] == &particle)
		other = &globalParticles[particles[1]];
	else other = &globalParticles[particles[0]];
	return (particle.pos - other->pos).normalized();
}

void DistanceConstraint::calculateError(std::vector<Particle>& globalParticles)
{
	float dist = (globalParticles[particles[0]]).pos.distance((globalParticles[particles[1]]).pos);
	error = (globalParticles[particles[0]]).pos.distance((globalParticles[particles[1]]).pos) - distance;
}