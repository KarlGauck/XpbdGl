#include <vector>

#include "constraintmanager.h"
#include "collisionconstraint.h"
#include "distanceconstraint.h"
#include "densityconstraint.h"
#include "maxpositionconstraint.h"
#include "collisionconstrainthandler.h"
#include "densityconstrainthandler.h"
#include "spatialhashgrid.h"

#include "profiler.h"

static Profiler profiler("Solver", 1000);
SpatialHashGrid ConstraintManager::grid = SpatialHashGrid(15.f);

void ConstraintManager::solveConstraints(std::vector<Particle>& particles, float dt)
{

	grid.updateParticles(particles);
	CollisionConstraintHandler::updateConstraints(particles, grid);
	DensityConstraintHandler::updateConstraints(particles, grid);


	//profiler.start();
	CollisionConstraint::solve(particles, dt);
	DensityConstraint::solve(particles, grid, dt);
	DistanceConstraint::solve(particles, dt);
	MaxPositionConstraint::solve(particles, dt);
	//profiler.end(particles.size());
}
