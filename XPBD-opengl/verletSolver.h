#pragma once

#include "solver.h"
#include "particle.h"

class VerletSolver : public Solver
{
public:
	void solve(float dt) override;
	void addParticle(Vec2 pos, Vec2 vel) override;
	std::vector<InstanceData> getCircleData() override;

private:
	std::vector<Particle> particles;

	bool collide(int particle1, int particle2);
	void solveCollisions();
};