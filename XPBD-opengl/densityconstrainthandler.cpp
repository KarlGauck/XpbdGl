#pragma once

#include <iostream>

#include "densityconstrainthandler.h"
#include "densityconstraint.h"

void DensityConstraintHandler::updateConstraints(std::vector<Particle>& particles, SpatialHashGrid grid)
{
	DensityConstraint::constraints.clear();

	for (int particleIndex = 0; particleIndex < particles.size(); particleIndex++)
	{
		Particle& particle = particles[particleIndex];
		std::vector<int> neighbours;
		for (int dx = 1; dx > -1; dx--)
			for (int dy = 1; dy > -1; dy--)
			{
				//std::vector<int> cellParticles = grid.getParticles(particle.pos + Vec2(dx * grid.cellSize, dy * grid.cellSize));
				for (int p = 0; p < particles.size(); p++/* : cellParticles*/)
				{
					if (particle.pos.x == particles[p].pos.x && particle.pos.y == particles[p].pos.y)
						continue;
					if (particle.pos.distance(particles[p].pos) >= DensityConstraint::KernelRadius)
						continue;
					neighbours.push_back(p);
				}
			}
		DensityConstraint::add(particleIndex, neighbours);
	}
}
