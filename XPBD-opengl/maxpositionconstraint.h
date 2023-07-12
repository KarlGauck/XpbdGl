#pragma once

#include <vector>

#include "particle.h"
#include "Vec2.h"

class MaxPositionConstraint
{
public:
	const static int ConstraintCountMax = 100000;
	static MaxPositionConstraint constraints[ConstraintCountMax];

	const static int ParticleCountMax = 1;
	
	static void solve(float dt);
	static void add(Particle* particles[ParticleCountMax], float xMax, float yMax, float xMin, float yMin);
public:
	Particle* particles[ParticleCountMax];
	Vec2 gradient[ParticleCountMax];

	Vec2 calculateGradient(Particle& particle);

	void calculateError();

	MaxPositionConstraint();
private:
	MaxPositionConstraint(Particle* particles[ParticleCountMax], float xMax, float yMax, float xMin, float yMin);

	static int currentIndex;
	float compliance;

	float error;
	float lambda;

	float xMax, yMax;
	float xMin, yMin;
};
