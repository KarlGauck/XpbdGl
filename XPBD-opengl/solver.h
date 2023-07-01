#pragma once

#include <vector>
#include <map>
#include <tuple>

#include "Vec2.h"
#include "constraint.h"
#include "particle.h"
#include "particlemanager.h"

class Solver {
public:
	Solver();
	void solve();
	void addParticle(Vec2 pos);
	std::vector<float> getPositions();
private:
	std::vector<Particle*> particles;
	std::vector<Constraint*> constraints;
	ParticleManager particleManager;
};



