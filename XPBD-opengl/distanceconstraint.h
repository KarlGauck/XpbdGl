#pragma once

#include <vector>

#include "particle.h"
#include "Vec2.h"

class DistanceConstraint
{
public:
	const static int ConstraintCountMax = 100;
	static DistanceConstraint constraints[ConstraintCountMax];

	const static int ParticleCountMax = 2;
	
	static void solve(float dt);
	static void add(Particle* particles[ParticleCountMax], float distance);
public:
	Particle* particles[ParticleCountMax];
	Vec2 gradient[ParticleCountMax];

	Vec2 calculateGradient(Particle& particle);

	void calculateError();

	DistanceConstraint();

private:
	DistanceConstraint(Particle* particles[ParticleCountMax], float distance);

	static int currentIndex;
	float compliance;

	float error;
	float lambda;

	float distance;
};