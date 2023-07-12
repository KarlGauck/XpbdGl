#include <vector>

#include "constraintmanager.h"
#include "collisionconstraint.h"
#include "distanceconstraint.h"
#include "maxpositionconstraint.h"
#include "collisionconstrainthandler.h"

void ConstraintManager::solveConstraints(std::vector<Particle*>& particles, float dt)
{
	//CollisionConstraintHandler::updateConstraints(particles);

	CollisionConstraint::solve(dt);
	DistanceConstraint::solve(dt);
	MaxPositionConstraint::solve(dt);
}
