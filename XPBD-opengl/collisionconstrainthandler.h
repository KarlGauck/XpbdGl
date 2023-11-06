#pragma once

#include <vector>

#include "spatialhashgrid.h"
#include "particle.h"

class CollisionConstraintHandler
{
public:
	static void updateConstraints(std::vector<Particle>& particles);

private:
	static SpatialHashGrid grid;
	static std::vector<int> returnParticles;
	static std::vector<int> processedParticles;
	static std::vector<int> neighbourParticles;
};