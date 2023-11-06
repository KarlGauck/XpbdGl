#include "collisionconstrainthandler.h"
#include "collisionconstraint.h"
#include "spatialhashgrid.h"
#include <chrono>
#include <iostream>
#include <numeric>

SpatialHashGrid CollisionConstraintHandler::grid;
std::vector<int> CollisionConstraintHandler::processedParticles;
std::vector<int> CollisionConstraintHandler::returnParticles;
std::vector<int> CollisionConstraintHandler::neighbourParticles;

void CollisionConstraintHandler::updateConstraints(std::vector<Particle>& globalParticles)
{
	CollisionConstraint::clear();

	/*
	for (int i = 0; i < globalParticles.size(); i++)
	{
		for (int j = 0; j < globalParticles.size(); j++)
		{
			if (i == j)
				continue;
			int vec[2] = { i, j };
			CollisionConstraint::add(vec);
		} 
	}
	return;
	*/

	grid.updateParticles(globalParticles);
	
	int constraintCount = 0;
	
	processedParticles.clear();

	for (int particleIndex = 0; particleIndex < globalParticles.size(); particleIndex++)
	{
		Particle& particle = globalParticles[particleIndex];
		neighbourParticles.clear();
		for (int dx = -1; dx <= 1; dx++)
			for (int dy = -1; dy <= 1; dy++)
			{
				Vec2 checkPos = particle.pos + Vec2(dx, dy);
				std::vector<int> cellParticles = grid.getParticles(checkPos);

				for (int particle : cellParticles) {
					if (std::find(neighbourParticles.begin(), neighbourParticles.end(), particle) != neighbourParticles.end())
						continue;
					neighbourParticles.push_back(particle);
				}
			}

		for (int cellParticle : neighbourParticles)
		{
			if (cellParticle == particleIndex)
				continue;
			for (int i = 0; i < processedParticles.size(); i++)
				if (processedParticles[i] == cellParticle)
					;// continue;
			
			int constraintParticles[2] = { particleIndex, cellParticle };
			CollisionConstraint::add(constraintParticles);
			constraintCount++;
		}
		processedParticles.push_back(particleIndex);
	}
}
