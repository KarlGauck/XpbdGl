#include <vector>
#include <iostream>

#include "collisionconstraintmanager.h"
#include "constraint.h"
#include "collisionconstraint.h"

CollisionConstraintManager::CollisionConstraintManager(std::vector<Particle*> particles)
	: ConstraintManager(particles)
{}

std::vector<Constraint*> CollisionConstraintManager::calculateConstraints()
{
	std::vector<Constraint*> constraints;

	grid.updateParticles(particles);

	std::vector<Particle*> processedParticles;
	for (Particle* particle : particles)
	{
		std::vector<Particle*> neighbourParticles;
		for (int dx = -1; dx <= 1; dx++)
			for (int dy = -1; dy <= 1; dy++)
			{
				Vec2 checkPos = particle->pos + Vec2(dx, dy);
				std::vector<Particle*> cellParticles = grid.getParticles(checkPos);
				for (Particle* ptr : cellParticles) {
					if (std::find(neighbourParticles.begin(), neighbourParticles.end(), ptr) != neighbourParticles.end())
						continue;
					neighbourParticles.push_back(ptr);
				}
			}

		for (Particle* cellParticle : neighbourParticles)
		{
			if (cellParticle == particle)
				continue;
			if (std::find(processedParticles.begin(), processedParticles.end(), cellParticle) != processedParticles.end())
				continue;
			
			std::vector<Particle*> constraintParticles;
			constraintParticles.push_back(particle);
			constraintParticles.push_back(cellParticle);

			constraints.push_back(
				new CollisionConstraint(
					constraintParticles
				)
			);
		}
		processedParticles.push_back(particle);
	}

	return constraints;
}