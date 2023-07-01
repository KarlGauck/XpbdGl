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

Solver::Solver() 
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

		for (int particleIndex = 0; particleIndex < particles.size(); particleIndex++)
		{
			Particle& particle = *particles[particleIndex];
			particle.vel = (particle.pos - oldPositions[particleIndex]) / dt;
		}

		particleManager.updateParticles();

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
	Particle& particle = *particles[particles.size() - 1];
	particleManager.addParticle(&particle);

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

	for (Particle* particlePtr : particles)
	{
		if (particlePtr == particles[particles.size() - 1])
			continue;
		std::vector<Particle*> p2;
		p2.push_back(particlePtr);
		p2.push_back(particles[particles.size() - 1]);
		constraints.push_back(
			new CollisionConstraint(
				p2
			)
		);
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


