#pragma once

#include <vector>

#include "particle.h"
#include "Vec2.h"

class CollisionConstraint
{
public:
	const static int ConstraintCountMax = 10000;
	static CollisionConstraint constraints[ConstraintCountMax];

	const static int ParticleCountMax = 2;
	
	static void solve(float dt);
	static void add(Particle* particles[ParticleCountMax]);
public:
	Vec2 calculateGradient(Particle& particle);

	void calculateError();

	Particle* particles[ParticleCountMax];
	Vec2 gradient[ParticleCountMax];

	CollisionConstraint();

private:
	CollisionConstraint(Particle* particles[ParticleCountMax]);

	static int currentIndex;
	float compliance;

	float error;
	float lambda;
};