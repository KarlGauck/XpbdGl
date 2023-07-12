#include "constraintmanager.h"
#include "collisionconstraint.h"
#include "distanceconstraint.h"
#include "maxpositionconstraint.h"

void ConstraintManager::solveConstraints(float dt)
{
	CollisionConstraint::solve(dt);
	DistanceConstraint::solve(dt);
	MaxPositionConstraint::solve(dt);
}
