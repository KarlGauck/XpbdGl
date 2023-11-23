#include "densityconstraint.h"

#include <iostream>
#include <algorithm>

std::vector<DensityConstraint> DensityConstraint::constraints = std::vector<DensityConstraint>();
float DensityConstraint::KernelRadius = 20.2f;
float DensityConstraint::RestDensity = 10.1f;

// IMPORTANT: First particle in list is the particle that is going to be modified, 
// all the others are just for 
DensityConstraint::DensityConstraint(int particle, std::vector<int>& neighbours)
	: particle(particle), neighbours(neighbours), error(0), lambda(0)
{}

void DensityConstraint::calculateGradient(std::vector<Particle>& globalParticles)
{
	Vec2 grad;

	Particle& part = globalParticles[particle];
	for (int neighbourIndex : neighbours)
	{
		Particle& neighbour = globalParticles[neighbourIndex];
		float radius = part.pos.distance(neighbour.pos);
		Vec2 dir = (neighbour.pos - part.pos) / radius;
		grad += dir * Poly6KernelGrad(radius);
	}
	gradient = grad / RestDensity;
}

void DensityConstraint::calculateError(std::vector<Particle>& globalParticles)
{
	error = (globalParticles[particle].density / RestDensity) - 1;
}

void DensityConstraint::solve(std::vector<Particle>& particles, SpatialHashGrid grid, float dt)
{
	calculateDensity(particles, grid);

	for (DensityConstraint& constraint : constraints)
	{
		// Calculate Error
		constraint.calculateError(particles);
		constraint.calculateGradient(particles);

		Particle& particle = particles[constraint.particle];

		// Calculate Lamba
		float weightedMass = 0.f;
		weightedMass += (1 / particle.mass) * constraint.gradient.dot(constraint.gradient);

		constraint.lambda = -constraint.error / (weightedMass + (constraint.compliance / (float)pow(dt, 2)));

	}
	for (DensityConstraint& constraint: constraints) 
	{
		Particle& particle = particles[constraint.particle];
		Vec2 deltaPos = constraint.gradient * constraint.lambda * (particle.mass);
		/*
		Vec2 deltaPos = Vec2(0.0f, 0.0f);
		for (int neighbourIndex = 0; neighbourIndex < constraint.neighbours.size(); neighbourIndex++)
		{
			float radius = particles[constraints[constraint.neighbours[neighbourIndex]].particle].pos.distance(particles[constraint.particle].pos);
			Vec2 dir = (particles[constraints[neighbourIndex].particle].pos - particles[constraint.particle].pos) / radius;
			deltaPos += dir * SpikyKernelGrad(radius) * (constraint.lambda + constraints[neighbourIndex].lambda);
		}*/
		deltaPos /= RestDensity;
		particle.oldPos = particle.pos;
		particle.pos += deltaPos;
	}
}

void DensityConstraint::calculateDensity(std::vector<Particle>& particles, SpatialHashGrid grid)
{
	for (int particleIndex = 0; particleIndex < particles.size(); particleIndex++)
	{
		Particle& particle = particles[particleIndex];
		particle.density = 0;
		for (int neighbourIndex = 0; neighbourIndex < particles.size(); neighbourIndex++)
		{
			if (neighbourIndex == particleIndex)
				continue;
			Particle& neighbour = particles[neighbourIndex];
			float radius = particle.pos.distance(neighbour.pos);
			if (radius < KernelRadius)
				particle.density += neighbour.mass * Poly6Kernel(radius);
		}
	}
}

void DensityConstraint::add(int particle, std::vector<int> neighbours)
{
	constraints.push_back(DensityConstraint(particle, neighbours));
}

float DensityConstraint::Poly6Kernel(float radius)
{
	if (radius >= KernelRadius) return 0;

	float volume = 3.1415926535 * pow(KernelRadius, 8) / 4;
	float val = std::max(0.0f, KernelRadius*KernelRadius - radius*radius);
	return val * val * val / volume;
}

float DensityConstraint::SpikyKernelGrad(float radius)
{
	if (radius >= KernelRadius) return 0;
	float scale = 12 / (pow(radius, 4) * 3.1415926535);
	return (radius - KernelRadius) * scale;
}

float DensityConstraint::Poly6KernelGrad(float radius)
{
	if (radius >= KernelRadius) return 0;
	return (-24 * radius * pow(pow(KernelRadius, 2) - pow(radius, 2), 2) / (3.1415926535 * pow(KernelRadius, 8)));
}