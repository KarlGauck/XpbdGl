#pragma once

#include <vector>

#include "particle.h"
#include "Vec2.h"

class DistanceConstraint
{
public:
	const static int ConstraintCountMax = 10000;
	static DistanceConstraint constraints[ConstraintCountMax];

	const static int ParticleCountMax = 2;
	
	static void solve(std::vector<Particle>& globalParticles, float dt);
	static void add(int particles[ParticleCountMax], float distance);
public:
	int particles[ParticleCountMax];
	Vec2 gradient[ParticleCountMax];

	Vec2 calculateGradient(std::vector<Particle>& globalParticles, Particle& particle);

	void calculateError(std::vector<Particle>& globalParticles);

	DistanceConstraint();

private:
	DistanceConstraint(int particles[ParticleCountMax], float distance);

	static int currentIndex;
	float compliance;

	float error;
	float lambda;

	float distance;
};