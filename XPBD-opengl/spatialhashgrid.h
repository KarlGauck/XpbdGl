#pragma once

#include <tuple>
#include <vector>

#include "particle.h"

class SpatialHashGrid
{
public:
	SpatialHashGrid();

	void updateParticles(std::vector<Particle*> &particles);

	std::vector<Particle*> getParticles(Vec2 pos);
private:
	int buckets[1000];
	std::vector<Particle*> particlePtrs;

	int hash(Vec2 pos);
};