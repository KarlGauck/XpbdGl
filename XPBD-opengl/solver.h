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
	std::vector<float> getPositions();
private:
	std::vector<Particle*> particles;

	void moveParticles(float dt);
};



