#include <vector>

#include "constraintmanager.h"
#include "collisionconstraint.h"
#include "distanceconstraint.h"
#include "maxpositionconstraint.h"
#include "collisionconstrainthandler.h"

#include "profiler.h"

static Profiler profiler("Solver", 1000);

void ConstraintManager::solveConstraints(std::vector<Particle>& particles, float dt)
{
	CollisionConstraintHandler::updateConstraints(particles);

	//profiler.start();
	CollisionConstraint::solve(particles, dt);
	DistanceConstraint::solve(particles, dt);
	MaxPositionConstraint::solve(particles, dt);
	//profiler.end(particles.size());
}
