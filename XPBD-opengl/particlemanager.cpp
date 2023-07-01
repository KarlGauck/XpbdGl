#include <map>
#include <tuple>
#include <vector>

#include "particlemanager.h"
#include "particle.h"

ParticleManager::ParticleManager()
{}

void ParticleManager::updateParticles()
{
	for (auto &cell : particleGrid)
	{
		std::vector<Particle*>& particles = cell.second;
		int xPos = cell.first.first, yPos = cell.first.second;
		for (auto it = particles.begin() ; it != particles.end(); it++)
		{
			Particle* particlePtr = *it;
			int newXPos = (int)(particlePtr->pos.x);
			int newYPos = (int)(particlePtr->pos.y);

			if (newXPos == xPos && newXPos == yPos)
				continue;

			auto oldKey = std::make_pair( xPos, yPos );
			particleGrid[oldKey].erase(it);

			auto newKey = std::make_pair( newXPos, newYPos );
			particleGrid[newKey].push_back(particlePtr);
		}
	}
}

void ParticleManager::addParticle(Particle* ptr)
{
	auto key = std::make_pair((int)ptr->pos.x, (int)ptr->pos.y);
	particleGrid[key].push_back(ptr);
}