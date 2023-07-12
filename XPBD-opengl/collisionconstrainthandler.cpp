#include "collisionconstrainthandler.h"
#include "collisionconstraint.h"
#include "spatialhashgrid.h"

SpatialHashGrid CollisionConstraintHandler::grid;

void CollisionConstraintHandler::updateConstraints(std::vector<Particle*> particles)
{
	CollisionConstraint::clear();
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
			
			Particle* constraintParticles[2] = { particle, cellParticle };
			CollisionConstraint::add(constraintParticles);
		}
		processedParticles.push_back(particle);
	}
}
