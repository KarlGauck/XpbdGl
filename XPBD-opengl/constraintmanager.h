#pragma once

#include <vector>

#include "particle.h"

class ConstraintManager
{
public:
	static void solveConstraints(std::vector<Particle>& particles, float dt);
};