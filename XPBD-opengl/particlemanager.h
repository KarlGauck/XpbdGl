#pragma once

#include <map>
#include <tuple>
#include <vector>

#include "particle.h"

class ParticleManager
{
public:
	ParticleManager();

	void updateParticles();
	void addParticle(Particle* ptr);

private:
	std::map<std::pair<int, int>, std::vector<Particle* >> particleGrid = {};
};