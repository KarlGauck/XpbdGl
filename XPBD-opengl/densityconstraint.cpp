#include "densityconstraint.h"

#include <iostream>
#include <algorithm>

#define M_PI 3.1415926535

std::vector<DensityConstraint> DensityConstraint::constraints = std::vector<DensityConstraint>();
float DensityConstraint::KernelRadius = 6.0f;
float DensityConstraint::RestDensity = 0.15f;
float DensityConstraint::EpsilonRelaxation = 0.6;
float DensityConstraint::DoubleCosKVal = 1.2;

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
		grad += dir * DoubleCosKernelGrad(radius);
	}
	gradient = grad / RestDensity;
}

void DensityConstraint::calculateError(std::vector<Particle>& globalParticles)
{
	error = (globalParticles[particle].density / RestDensity) - 1;
	//std::cout << "Density: " << globalParticles[particle].density << " Error: " << error << std::endl;
}

void DensityConstraint::solve(std::vector<Particle>& particles, SpatialHashGrid grid, float dt)
{
	calculateDensity(particles, grid);

	std::vector<float> gradLengths(particles.size());
	std::vector<float> lambdas(particles.size());

	for (DensityConstraint& constraint : constraints)
	{
		// Calculate Error
		constraint.calculateError(particles);
		constraint.calculateGradient(particles);
		gradLengths[constraint.particle] = constraint.gradient.length();
	}

	for (DensityConstraint& constraint : constraints)
	{
		Particle& particle = particles[constraint.particle];
		

		//constraint.lambda = -constraint.error / (weightedMass + (constraint.compliance / (float)pow(dt, 2)));

		float gradientLenSum = 0.0;
		for (int part : constraint.neighbours)
			gradientLenSum += gradLengths[part] * gradLengths[part];
		if (gradientLenSum == 0)
			gradientLenSum = 1000000;

		// Calculate Lamba
		float weightedMass = 0.f;
		weightedMass += (1 / particle.mass) * gradientLenSum;

		//constraint.lambda = constraint.error / gradientLenSum;
		constraint.lambda = constraint.error / (EpsilonRelaxation + weightedMass + (constraint.compliance / (float)pow(dt, 2)));
		lambdas[constraint.particle] = constraint.lambda;
	}

	std::vector<Vec2> deltaPositions(particles.size());

	for (DensityConstraint& constraint: constraints) 
	{
		Particle& particle = particles[constraint.particle];

		Vec2 deltaPos = Vec2(0, 0);
		for (int part : constraint.neighbours)
		{
			Vec2 relPos = particles[constraint.particle].pos - particles[constraints[part].particle].pos;
			float len = relPos.length();
			float gradAbs = DoubleCosKernel(len);
			deltaPos += (relPos / len) * gradAbs * (constraint.lambda + lambdas[part]);
		}
		deltaPos *= particle.mass / RestDensity;
		deltaPositions[constraint.particle] = deltaPos;

		//particle.oldPos = particle.pos;
		//particle.pos += deltaPos;
	}

	for (DensityConstraint& constraint: constraints) 
	{
		Particle& part = particles[constraint.particle];
		part.oldPos = part.pos;
		part.pos += deltaPositions[constraint.particle];
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
			if (radius < KernelRadius) {
				particle.density += neighbour.mass * DoubleCosKernel(radius);
				//std::cout << "lel : " << DoubleCosKernel(radius) << std::endl;
			}
		}
		//std::cout << particle.density << std::endl;
	}
}

void DensityConstraint::add(int particle, std::vector<int> neighbours)
{
	constraints.push_back(DensityConstraint(particle, neighbours));
}


float DensityConstraint::SpikyKernel(float radius)
{
	if (radius >= KernelRadius) return 0;

	float volume = (3.1415926535 * pow(KernelRadius, 4)) / 6;
	return (KernelRadius - radius) * (KernelRadius - radius) / volume;
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

float DensityConstraint::DoubleCosKernel(float r)
{
	float h = DensityConstraint::KernelRadius;
	float k = DoubleCosKVal;

	float s = r / h;
	if (k < s)
		return 0;
	float sigma2d = M_PI / ((3 * pow(M_PI, 2) - 16) * pow(k * h, 2));

	float val = 4 * cos(s * M_PI / k) + cos(2 * M_PI * s / k) + 3;
	return val * sigma2d;
}

float DensityConstraint::DoubleCosKernelGrad(float r)
{
	float h = DensityConstraint::KernelRadius;
	float k = DoubleCosKVal;

	float s = r / h;
	if (k < s)
		return 0;
	float sigma2d = M_PI / ((3 * pow(M_PI, 2) - 16) * pow(k * h, 2));

	float val = -(4*M_PI/k) * sin(s * M_PI / k)/h - (2*M_PI/k) * sin(2 * M_PI * s / k)/h;
	return val * sigma2d;
}