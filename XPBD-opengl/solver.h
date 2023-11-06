#pragma once

#include <vector>
#include <map>
#include <tuple>

#include "Vec2.h"
#include "particle.h"

class Solver {
public:
	Solver();
	void solve();
	void addParticle(Vec2 pos);

	void addSoftBody(Vec2 pos);

	std::vector<float> getPositions();
	void calculateDensity();

	std::vector<Particle> particles;
private:
	std::vector<Vec2> oldPositions;

	void moveParticles(float dt);
};



