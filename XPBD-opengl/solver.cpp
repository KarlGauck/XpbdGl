#pragma once

#include <iostream>
#include <tuple>
#include <map>

#include "Vec2.h"
#include "solver.h"
#include "particle.h"
#include "collisionconstraint.h"
#include "distanceconstraint.h"
#include "maxpositionconstraint.h"
#include "constraintmanager.h"

Solver::Solver(){
}

void Solver::solve() 
{
	float dtUncorrected = 0.03f;

	float substepNumber = 10;
	for (int substep = 0; substep < substepNumber; substep++) {
		float dt = dtUncorrected / substepNumber;

		Vec2 gravityAccel(0.f, 9.81f);
		std::vector<Vec2> oldPositions;

		for (int particleIndex = 0; particleIndex < particles.size(); particleIndex++) {
			Particle& particle = *particles[particleIndex];

			oldPositions.push_back(particle.pos);

			particle.vel -= gravityAccel * dt;
			particle.pos += particle.vel * dt;
		}

		ConstraintManager::solveConstraints(particles, dt);

		for (int particleIndex = 0; particleIndex < particles.size(); particleIndex++)
		{
			Particle& particle = *particles[particleIndex];
			particle.vel = (particle.pos - oldPositions[particleIndex]) / dt;
		}
	}

}

void Solver::addParticle(Vec2 pos)
{
	particles.push_back(
		new Particle(
			1.0f,
			pos,
			Vec2(0.0f, 0.0f)
		)
	);

	Particle* particlePtr = particles[particles.size() - 1];

	Particle* p1[1];
	p1[0] = particlePtr;
	std::cout << p1[0] << std::endl;
	MaxPositionConstraint::add(
		p1,
		14.5f,
		7.5f,
		-14.5f,
		-7.5f
	);

	for (Particle* ptr : particles)
	{
		if (ptr == particlePtr)
			continue;
		Particle* collisionParticles[2] = { ptr, particlePtr };
		CollisionConstraint::add(collisionParticles);
	}
}

std::vector<float> Solver::getPositions() 
{
	std::vector<float> positions;
	for (Particle* particlePtr : particles) {
		Particle& particle = *particlePtr;
		positions.push_back(particle.pos.x);
		positions.push_back(particle.pos.y);

		positions.push_back(1.0f);
		positions.push_back(1.0f);
		positions.push_back(0.0f);
	}
	return positions;
}
