#include <vector>
#include <iostream>

#include "collisionconstraint.h"
#include "particle.h"

int CollisionConstraint::currentIndex = 0;
CollisionConstraint CollisionConstraint::constraints[ConstraintCountMax];

CollisionConstraint::CollisionConstraint() {};

CollisionConstraint::CollisionConstraint(Particle* particles[ParticleCountMax]): compliance(0.0f)
{
	for (int i = 0; i < ParticleCountMax; i++)
	{
		this->particles[i] = particles[i];
	}
};

void CollisionConstraint::add(Particle* particles[ParticleCountMax])
{
	if (currentIndex >= ConstraintCountMax)
		return;
	constraints[currentIndex] = CollisionConstraint(particles);
	currentIndex++;
}

void CollisionConstraint::solve(float dt)
{
	for (int constraintIndex = 0; constraintIndex < currentIndex; constraintIndex++)
	{
		// Calculate Gradients
		CollisionConstraint& constraint = constraints[constraintIndex];
		for (int particleIndex = 0; particleIndex < ParticleCountMax; particleIndex++)
			constraint.gradient[particleIndex] = constraint.calculateGradient(*constraint.particles[particleIndex]);

		// Calculate Error
		constraint.calculateError();

		// Calculate Lamba
		float weightedMass = 0.f;
		for (int particleIndex = 0; particleIndex < ParticleCountMax; particleIndex++)
		{
			Particle& particle = *constraint.particles[particleIndex];
			Vec2 grad = constraint.gradient[particleIndex];
			weightedMass += (1/particle.mass) * grad.dot(grad);
		}
		constraint.lambda = -constraint.error / (weightedMass + (constraint.compliance/(float)pow(dt, 2)));

		for (int particleIndex = 0; particleIndex < ParticleCountMax; particleIndex++)
		{
			Particle& particle = *constraint.particles[particleIndex];
			Vec2 deltaPos = constraint.gradient[particleIndex] * constraint.lambda * (1/particle.mass);
			particle.oldPos = particle.pos;
			particle.pos += deltaPos;
		}
	}

}

Vec2 CollisionConstraint::calculateGradient(Particle& particle)
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
