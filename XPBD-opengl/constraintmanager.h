#pragma once

#include <vector>

#include "particle.h"
#include "spatialhashgrid.h"

class ConstraintManager
{
public:
	static void solveConstraints(std::vector<Particle>& particles, float dt);

private:
	static SpatialHashGrid grid;
};