#pragma once

#include <tuple>
#include <vector>

#include "particle.h"

class SpatialHashGrid
{
public:
	SpatialHashGrid();

	void updateParticles(std::vector<Particle>& globalParticles);

	std::vector<int> getParticles(Vec2 pos);
	std::vector<int> resultParticles();
private:
	const static int PARTICLE_COUNT_RESERVE = 10000;

	int buckets[100];
	std::vector<int> particles;
	int runningIndex;

	int hash(Vec2 pos);
};