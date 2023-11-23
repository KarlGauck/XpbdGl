#pragma once

#include <vector>
#include <map>
#include <tuple>

#include "solver.h"

#include "Vec2.h"
#include "particle.h"

class XpbdSolver : public Solver{
public:
	XpbdSolver();
	void solve(float dt) override;
	void addParticle(Vec2 pos, Vec2 vel) override;

	void addSoftBody(Vec2 pos);
	std::vector<InstanceData> getCircleData() override;

	float getDensity(Vec2 pos);
	Vec2 getPressureForce(Particle& part, float dt);

	std::vector<Particle> particles;
	bool gravity = true;
private:
	std::vector<Vec2> oldPositions;

	void moveParticles(float dt);
};



