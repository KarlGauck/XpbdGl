#pragma once

#include <iostream>
#include <tuple>
#include <map>
#include <cmath>
#include <algorithm>

#include "Vec2.h"
#include "solver.h"
#include "particle.h"
#include "collisionconstraint.h"
#include "distanceconstraint.h"
#include "maxpositionconstraint.h"
#include "constraintmanager.h"
#include "profiler.h"

# define M_PI 3.14159265358979323846

Solver::Solver() {
	int MAX_PARTIClES = 10000;
	particles.reserve(MAX_PARTIClES);
	oldPositions.reserve(MAX_PARTIClES);
}

static int count = 0;

void Solver::solve(float deltaTime) 
{

	count = (count + 1) % 5;
	if (count == 0)
		addParticle(Vec2(-45, 20), Vec2(20.0f, -20.f));
	float substepNumber = 1;
	for (int substep = 0; substep < substepNumber; substep++) 
	{
		float dt = deltaTime / substepNumber;
		oldPositions.clear();

		Vec2 gravityAccel(0.f, 9.81f);

		for (int particleIndex = 0; particleIndex < particles.size(); particleIndex++) 
		{
			Particle& particle = particles[particleIndex];

			oldPositions.push_back(particle.pos);

			particle.vel -= gravityAccel * dt;
			particle.pos += particle.vel * dt + gravityAccel * dt * dt * .5f;
		}

		ConstraintManager::solveConstraints(particles, dt);
		//calculateSPH();

		for (int particleIndex = 0; particleIndex < particles.size(); particleIndex++)
		{
			Particle& particle = particles[particleIndex];
			particle.vel = (particle.pos - oldPositions[particleIndex]) / dt;
		}
	}
}

void Solver::addSoftBody(Vec2 pos)
{
	int width = 7;
	int height = 7;
	float gap = 1.3;
	for (int x = -width/2; x <= width/2; x++)
	{
		for (int y = -height/2; y <= height/2; y++)
		{
			addParticle(pos + Vec2(x * gap, y * gap), Vec2(0.0f, 0.0f));
			int index = particles.size() - 1;


			/* BUGGY CODE: WILL CRASH, DO NOT USE!!!!
			for (int dx = -1; dx <= 0; dx++) {
				for (int dy = -1; dy <= 0; dy++) {
					bool run = true;
					if (dx == 0 && dy == 0)
						run = false;
					if (x == -width / 2 && dx != 0)
						run = false;
					if (y == -width / 2 && dy != 0)
						run = false;
					if (!run)
					{
						std::cout << "x: " << x << " dx: " << dx << " y: " << y << " dy: " << dy << std::endl;
						continue;
					}
					int nIndex = index + dy * width + dx;
					std::cout << nIndex << " size: " << particles.size();
					int parts[2] = {index, nIndex};
					DistanceConstraint::add(parts, sqrt((dx*gap)*(dx*gap) + (dy*gap)*(dy*gap)));
				}
			}
			*/
		}
	}
}

void Solver::addParticle(Vec2 pos, Vec2 vel)
{
	float color[] = {
		static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX),
		static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX),
		static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX),
		static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)
	};
	particles.push_back(
		Particle(
			1.0f,
			0.5f,
			pos,
			vel,
			color
		)
	);

	int p1[1];
	p1[0] = particles.size() - 1;

	float width = 100;
	float height = 50;
	MaxPositionConstraint::add(
		p1,
		((width-1)/2),
		((height-1)/2),
		-((width-1)/2),
		-((height-1)/2)
	);

	for (int particleIndex = 0; particleIndex < particles.size(); particleIndex++)
	{
		if (particleIndex == particles.size()-1)
			continue;
		int collisionParticles[2] = { particleIndex, particles.size()-1};
		CollisionConstraint::add(collisionParticles);
	}
}

std::vector<float> Solver::getPositions() 
{
	std::vector<float> positions;
	for (Particle& particle : particles) {

		positions.push_back(particle.pos.x);
		positions.push_back(particle.pos.y);

		positions.push_back(particle.radius * 2);
		positions.push_back(particle.radius * 2);
		positions.push_back(1.0f);
		positions.push_back(0.0f);

		for (int i = 0; i < 4; i++)
			positions.push_back(particle.color[i]);
	}
	return positions;
}

/*
void Solver::calculateDensity()
{
	float simRadius = 3.0f;
	std::vector<float>  densities;

	for (Particle* part : particles)
	{
		part->density = 0.0f;
		for (Particle* other : particles) 
		{
			if (part == other)
				continue;
			float dist = part->pos.distance(other->pos);
			part->density += kernel(simRadius, dist) + other->mass;
		}
	}
}

float kernel(float radius, float dist)
{
	float volume = M_PI * pow(radius, 8) / 4;
	float value = std::max(0.f, radius * radius - dist * dist);
	return value * value * value / volume;
}
*/
