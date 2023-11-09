#include <vector>
#include <iostream>

#include "collisionconstraint.h"
#include "particle.h"
#include "Vec2.h"
#include "profiler.h"

static Profiler profiler("Collision Solver", 1000);

int CollisionConstraint::currentIndex = 0;
CollisionConstraint CollisionConstraint::constraints[ConstraintCountMax];

CollisionConstraint::CollisionConstraint() {};

CollisionConstraint::CollisionConstraint(int particles[ParticleCountMax]): compliance(0.0f)
{
	for (int i = 0; i < ParticleCountMax; i++)
	{
		this->particles[i] = particles[i];
	}
};

void CollisionConstraint::add(int particles[ParticleCountMax])
{
	if (currentIndex >= ConstraintCountMax)
		return;
	constraints[currentIndex] = CollisionConstraint(particles);
	currentIndex++;
}

void CollisionConstraint::solve(std::vector<Particle>& globalParticles, float dt)
{
	profiler.start();

	for (int constraintIndex = 0; constraintIndex < currentIndex; constraintIndex++)
	{
		// Calculate Gradients
		CollisionConstraint& constraint = constraints[constraintIndex];
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
		constraint.lambda = -constraint.error / (weightedMass + (constraint.compliance/(float)pow(dt, 2)));

		for (int particleIndex = 0; particleIndex < ParticleCountMax; particleIndex++)
		{
			Particle& particle = globalParticles[constraint.particles[particleIndex]];
			Vec2 deltaPos = constraint.gradient[particleIndex] * constraint.lambda * (1/particle.mass);
			particle.oldPos = particle.pos;
			particle.pos += deltaPos;
		}
	}
	//profiler.end();

}

static float q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long*)&y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);               // what the fuck?
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
	// y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

Vec2 CollisionConstraint::calculateGradient(std::vector<Particle>& globalParticles, Particle& particle)
{
	Particle* other = nullptr;
	if (&globalParticles[particles[0]] == &particle)
		other = &globalParticles[particles[1]];
	else other = &globalParticles[particles[0]];
	//Vec2 temp = other->pos - particle.pos;
	//return Vec2(pow(temp.x, 2.0f), pow(temp.y, 2.0f));
	float dx = other->pos.x - particle.pos.x;
	float dy = other->pos.y - particle.pos.y;
	float len = dx*dx + dy*dy;
	return (other->pos - particle.pos) * (1 / sqrt(len));// q_rsqrt(len);
}

void CollisionConstraint::calculateError(std::vector<Particle>& globalParticles)
{
	float dx = globalParticles[particles[0]].pos.x - globalParticles[particles[1]].pos.x;
	float dy = globalParticles[particles[0]].pos.y - globalParticles[particles[1]].pos.y;
	float distance = dx * dx + dy * dy;
	distance *= (1 / sqrt(distance));// q_rsqrt(distance);
	float wantedDistance = globalParticles[particles[0]].radius + globalParticles[particles[1]].radius;
	if (distance < wantedDistance)
		error = wantedDistance-distance;
	else error = 0;
}

void CollisionConstraint::clear()
{
	currentIndex = 0;
}


