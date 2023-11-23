#pragma once

#include <vector>

#include "particle.h"
#include "spatialhashgrid.h"

class DensityConstraintHandler
{
public:
	static void updateConstraints(std::vector<Particle>& particles, SpatialHashGrid grid);
};