#pragma once

#include <vector>
#include <iostream>

#include "Vec2.h"
#include "particle.h"
#include "maxpositionconstraint.h"

int MaxPositionConstraint::currentIndex = 0;
MaxPositionConstraint MaxPositionConstraint::constraints[ConstraintCountMax];

MaxPositionConstraint::MaxPositionConstraint() {};

MaxPositionConstraint::MaxPositionConstraint
(int particles[ParticleCountMax], float xMax, float yMax, float xMin, float yMin) :
	xMax(xMax), yMax(yMax), xMin(xMin), yMin(yMin), compliance(0.001f)
{
	for (int i = 0; i < ParticleCountMax; i++)
	{
		std::cout << "Set: " << particles[i] << std::endl;
		this->particles[i] = particles[i];
	}
};

void MaxPositionConstraint::add(int particles[ParticleCountMax], float xMax, float yMax, float xMin, float yMin)
{
	if (currentIndex >= ConstraintCountMax)
		return;
	constraints[currentIndex] = MaxPositionConstraint(particles, xMax, yMax, xMin, yMin);
	currentIndex++;
}

void MaxPositionConstraint::solve(std::vector<Particle>& globalParticles, float dt)
{
	for (int constraintIndex = 0; constraintIndex < currentIndex; constraintIndex++)
	{
		// Calculate Gradients
		MaxPositionConstraint& constraint = constraints[constraintIndex];
		for (int particleIndex = 0; particleIndex < ParticleCountMax; particleIndex++)
			constraint.gradient[particleIndex] = constraint.calculateGradient(globalParticles[constraint.particles[particleIndex]]);

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

void MaxPositionConstraint::calculateError(std::vector<Particle>& globalParticles)
{
	Particle& particle = globalParticles[particles[0]];
	float capX = particle.pos.x;
	float capY = particle.pos.y;
	if (particle.pos.x > xMax)
		capX = xMax;
	if (particle.pos.x < xMin)
		capX = xMin;
	if (particle.pos.y > yMax)
		capY = yMax;
	if (particle.pos.y < yMin)
		capY = yMin;
	error = particle.pos.distance(Vec2(capX, capY));
}

Vec2 MaxPositionConstraint::calculateGradient(Particle& particle)
{
	float capX = particle.pos.x, capY = particle.pos.y;
	
	if (particle.pos.x > xMax)
		capX = xMax;
	if (particle.pos.x < xMin)
		capX = xMin;
	if (particle.pos.y > yMax)
		capY = yMax;
	if (particle.pos.y < yMin)
		capY = yMin;

	return (particle.pos - Vec2(capX, capY)).normalized();
};