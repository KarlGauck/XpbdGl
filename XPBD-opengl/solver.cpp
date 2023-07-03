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
#include "constraint.h"

Solver::Solver(): collisionManager(particles)
{
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

		solveConstraints(constraints, dt);

		auto additional = collisionManager.calculateConstraints();
		solveConstraints(additional, dt);

		for (int particleIndex = 0; particleIndex < particles.size(); particleIndex++)
		{
			Particle& particle = *particles[particleIndex];
			particle.vel = (particle.pos - oldPositions[particleIndex]) / dt;
		}

	}
}

void Solver::solveConstraints(std::vector<Constraint*>& constraints, float dt)
{
	for (int constraintIndex = 0; constraintIndex < constraints.size(); constraintIndex++) 
		{
			Constraint& constraint = *constraints[constraintIndex];

			constraint.calculateError();
			constraint.calculateLambda(dt);

			for (int particleIndex = 0; particleIndex < constraint.particles.size(); particleIndex++)
			{
				Particle& particle = *constraint.particles[particleIndex];
				Vec2 deltaPos = constraint.deltaPos(particle);
				particle.pos += deltaPos;
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
	collisionManager.addParticle(particles[particles.size() - 1]);

	std::vector<Particle*> p1;
	p1.push_back(particles[particles.size()-1]);
	constraints.push_back(
		new MaxPositionConstraint(
			p1,
			14.5f,
			7.5f,
			-14.5f,
			-7.5f
		)
	);

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