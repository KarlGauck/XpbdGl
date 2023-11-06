#pragma once

#include <vector>

#include "particle.h"
#include "Vec2.h"

class CollisionConstraint
{
public:
	const static int ConstraintCountMax = 1000000;
	static CollisionConstraint constraints[ConstraintCountMax];

	const static int ParticleCountMax = 2;
	
	static void solve(std::vector<Particle>& globalParticles, float dt);
	static void add(int particles[ParticleCountMax]);

	static void clear();
public:
	Vec2 calculateGradient(std::vector<Particle>& globalParticles, Particle& particle);

	void calculateError(std::vector<Particle>& globalParticles);

	int particles[ParticleCountMax];
	Vec2 gradient[ParticleCountMax];

	CollisionConstraint();

private:
	CollisionConstraint(int particles[ParticleCountMax]);

	static int currentIndex;
	float compliance;

	float error;
	float lambda;
};