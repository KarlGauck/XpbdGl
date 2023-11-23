#pragma once

#include <iostream>
#include <tuple>
#include <map>
#include <cmath>
#include <algorithm>

#include "Vec2.h"
#include "xpbdSolver.h"
#include "particle.h"
#include "collisionconstraint.h"
#include "distanceconstraint.h"
#include "maxpositionconstraint.h"
#include "constraintmanager.h"
#include "profiler.h"

# define M_PI 3.14159265358979323846

XpbdSolver::XpbdSolver() {
	int MAX_PARTIClES = 10000;
	particles.reserve(MAX_PARTIClES);
	oldPositions.reserve(MAX_PARTIClES);
}

static int count = 1;
static int MAX_COUNT = 1500;

static float SMOOTHING_RADIUS = 13.2f;
static float targetDensity = 0.00f;
static float pressureMultiplier = 18000.0f;

void XpbdSolver::solve(float deltaTime)
{
	//count++;
	if (count % 5 == 0 && particles.size() < MAX_COUNT)
		for (int i = 0; i < 5; i ++)
			addParticle(Vec2(-5 , 2 - i), Vec2(6.0f, -0.5f));
	float substepNumber = 1;
	for (int substep = 0; substep < substepNumber; substep++) 
	{
		float dt = deltaTime / substepNumber;
		oldPositions.clear();

		float fac = 1/1200.f;

		// Calculate Densities
		/*
		for (int particleIndex = 0; particleIndex < particles.size(); particleIndex++)
		{
			Particle& particle = particles[particleIndex];
			particle.density = getDensity(particle.pos + particle.vel*fac);
		}
		*/

		Vec2 gravityAccel(0.f, 9.81f);
		for (int particleIndex = 0; particleIndex < particles.size(); particleIndex++) 
		{
			Particle& particle = particles[particleIndex];
			oldPositions.push_back(particle.pos);



			// Apply pressureforce
			if (particle.density != 0)
				;// particle.vel = (getPressureForce(particle, fac) / particle.density) * dt;

			if (gravity && particleIndex != 0)
				particle.vel -= gravityAccel * dt;

			particle.pos += particle.vel * dt;
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

void XpbdSolver::addSoftBody(Vec2 pos)
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


void XpbdSolver::addParticle(Vec2 pos, Vec2 vel)
{
	float val = (particles.size() / (float)MAX_COUNT);
	float color[] = {
		val, (val * (1-val))*4, (1 - val), 1.0f
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

	float width = 200;
	float height = 100;
	MaxPositionConstraint::add(
		p1,
		((width-1)/2),
		((height-1)/2),
		-((width-1)/2),
		-((height-1)/2)
	);

	/*
	for (int particleIndex = 0; particleIndex < particles.size(); particleIndex++)
	{
		if (particleIndex == particles.size()-1)
			continue;
		int collisionParticles[2] = { particleIndex, particles.size()-1};
		CollisionConstraint::add(collisionParticles);
	}
	*/
}

std::vector<InstanceData> XpbdSolver::getCircleData() 
{
	std::vector<InstanceData> data;
	for (Particle& particle : particles)
		data.push_back(
			InstanceData{
				particle.pos,
				Vec2(particle.radius * 2, particle.radius * 2),
				Vec2(1.0f, 0.0f),
				{
					particle.color[0],
					particle.color[1],
					particle.color[2],
					particle.color[3],
				}
			}
		);
	return data;
}

static float kernelFunction_2(float radius, float distance)
{
	if (distance >= radius) return 0;

	float volume = 3.1415926535 * pow(radius, 8) / 4;
	float val = std::fmax(0, radius * radius - distance * distance);
	return val * val * val / volume;
}

static float kernelFunction(float radius, float distance)
{
	if (distance >= radius) return 0;

	float volume = (M_PI * pow(radius, 4)) / 6;
	return (radius - distance) * (radius - distance) / volume;
}

static float kernelGrad(float radius, float distance)
{
	if (distance >= radius) return 0;
	float scale = 12 / (pow(radius, 4) * M_PI);
	return (distance - radius) * scale;
}

static float kernelGrad_2(float radius, float distance)
{
	if (distance >= radius) return 0;
	return (-24 * distance * pow(pow(radius, 2) - pow(distance, 2), 2) / (3.1415926535 * pow(radius, 8)));
}



static float densityToPressure(float density)
{
	float error = density - targetDensity;
	return error * pressureMultiplier;
}


float XpbdSolver::getDensity(Vec2 pos)
{
	float density = 0.0f;
	for (Particle& particle : particles)
	{
		float dist = particle.pos.distance(pos);
		if (dist == 0)
			continue;
		density += kernelFunction(SMOOTHING_RADIUS, dist) * particle.mass;
	}
	return density;
}

Vec2 XpbdSolver::getPressureForce(Particle& part, float dt)
{
	Vec2 force = Vec2(0.0f, 0.0f);
	for (Particle& particle : particles)
	{
		float dist = particle.pos.distance(part.pos);
		if (dist > SMOOTHING_RADIUS)
			continue;
		if (particle.density == 0)
			continue;
		if (dist == 0)
			continue;

		float pressure1 = densityToPressure(particle.density);
		float pressure2 = densityToPressure(part.density);
		float sharedPressure = (pressure1 + pressure2) / 2;

		float fac = 0.03;
		Vec2 pn1 = particle.pos + particle.vel * dt;
		Vec2 pn2 = part.pos + part.vel * dt;

		force += ((pn1-pn2)/dist) * sharedPressure * kernelGrad(SMOOTHING_RADIUS, dist) * particle.mass/particle.density;
	}
	return force;
}

