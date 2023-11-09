#pragma once

#include <vector>

#include "particle.h"
#include "Vec2.h"

class MaxPositionConstraint
{
public:
	const static int ConstraintCountMax = 1000000;
	static MaxPositionConstraint constraints[ConstraintCountMax];

	const static int ParticleCountMax = 1;
	
	static void solve(std::vector<Particle>& globalParticles, float dt);
	static void add(int particles[ParticleCountMax], float xMax, float yMax, float xMin, float yMin);
public:
	int particles[ParticleCountMax];
	Vec2 gradient[ParticleCountMax];

	Vec2 calculateGradient(Particle& particle);

	void calculateError(std::vector<Particle>& globalParticles);

	MaxPositionConstraint();
private:
	MaxPositionConstraint(int particles[ParticleCountMax], float xMax, float yMax, float xMin, float yMin);

	static int currentIndex;
	float compliance;

	float error;
	float lambda;

	float xMax, yMax;
	float xMin, yMin;
};
