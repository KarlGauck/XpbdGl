#pragma once

#include <vector>

#include "constraintmanager.h"
#include "constraint.h"

class CollisionConstraintManager : public ConstraintManager
{
public:
	CollisionConstraintManager(std::vector<Particle*> particles);
	std::vector<Constraint*> calculateConstraints();
private:
	SpatialHashGrid grid;
};