#include <tuple>
#include <vector>
#include <iostream>

#include "spatialhashgrid.h"
#include "particle.h"

SpatialHashGrid::SpatialHashGrid()
{
}

void SpatialHashGrid::updateParticles(std::vector<Particle*> &particles)
{
	int bucketCount = sizeof(buckets) / sizeof(int);
	particlePtrs.clear();

	for (int i = 0; i < bucketCount; i++) 
		buckets[i] = 0;

	for (Particle* ptr : particles) 
	{
		int bucketIndex = hash(ptr->pos);
		buckets[bucketIndex]++;
		particlePtrs.push_back(nullptr);
	}

	for (int i = 1; i < bucketCount; i++)
	{
		buckets[i] += buckets[i - 1];
	}

	for (Particle* ptr : particles)
	{
		int bucketIndex = hash(ptr->pos);
		int particleCount = buckets[bucketIndex];
		if (bucketIndex > 0)
			particleCount -= buckets[bucketIndex - 1];

		int particleIndex = buckets[bucketIndex] - particleCount;
		for (int i = 0; i < particleCount; i++)
		{
			if (particlePtrs[particleIndex + i] != nullptr)
				continue;
			particlePtrs[particleIndex + i] = ptr;
			break;
		}
	}
}

int SpatialHashGrid::hash(Vec2 pos) 
{
	int bucketCount = (sizeof(buckets) / sizeof(int));
	int x = (int)pos.x;
	int y = (int)pos.y;
	int h = (x * 92837111) ^ (y * 689287499);
	return abs(h) % (bucketCount - 1);
}

std::vector<Particle*> SpatialHashGrid::getParticles(Vec2 pos)
{
	std::vector<Particle*> particles;

	int bucketIndex = hash(pos);
	int particleCount = buckets[bucketIndex];
	if (bucketIndex > 0)
		particleCount -= buckets[bucketIndex - 1];
	int index = buckets[bucketIndex] - particleCount;

	for (int i = index; i < index + particleCount; i++)
	{
		particles.push_back(particlePtrs[i]);
	}
	return particles;
}
