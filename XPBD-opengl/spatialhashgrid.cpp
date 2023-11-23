#include <tuple>
#include <vector>
#include <iostream>

#include "spatialhashgrid.h"
#include "particle.h"

SpatialHashGrid::SpatialHashGrid(float cellSize): cellSize(cellSize)
{
	particles.reserve(PARTICLE_COUNT_RESERVE);
}

void SpatialHashGrid::updateParticles(std::vector<Particle>& globalParticles)
{
	int bucketCount = sizeof(buckets) / sizeof(int);
	particles.clear();

	for (int i = 0; i < bucketCount; i++) 
		buckets[i] = 0;

	for (int particleIndex = 0; particleIndex < globalParticles.size(); particleIndex++) 
	{
		Particle& particle = globalParticles[particleIndex];
		int bucketIndex = hash(particle.pos);
		buckets[bucketIndex]++;
		particles.push_back(-1);
	}

	for (int i = 1; i < bucketCount; i++)
	{
		buckets[i] += buckets[i - 1];
	}

	for (int particleIndex = 0; particleIndex < globalParticles.size(); particleIndex++)
	{
		Particle& particle = globalParticles[particleIndex];
		int bucketIndex = hash(particle.pos);
		int particleCount = buckets[bucketIndex];
		if (bucketIndex > 0)
			particleCount -= buckets[bucketIndex - 1];

		int particleBucketIndex = buckets[bucketIndex] - particleCount;
		for (int i = 0; i < particleCount; i++)
		{
			if (particles[particleBucketIndex + i] != -1)
				continue;
			particles[particleBucketIndex + i] = particleIndex;
			break;
		}
	}

}

int SpatialHashGrid::hash(Vec2 pos) 
{
	int bucketCount = (sizeof(buckets) / sizeof(int));
	int x = (int)floor(pos.x/cellSize);
	int y = (int)floor(pos.y/cellSize);
	int h = (x * 92837111) ^ (y * 689287499);
	return abs(h) % (bucketCount - 1);
}

std::vector<int> SpatialHashGrid::getParticles(Vec2 pos)
{
	int runningIndex = 0;
	std::vector<int> resParticles;
	resParticles.reserve(100);

	int bucketIndex = hash(pos);
	int particleCount = buckets[bucketIndex];
	if (bucketIndex > 0)
		particleCount -= buckets[bucketIndex - 1];
	int index = buckets[bucketIndex] - particleCount;

	for (int i = index; i < index + particleCount; i++)
	{
		resParticles.push_back(particles[i]);
	}
	return resParticles;
}
