#pragma once

#include <vector>

#include "constraint.h"
#include "particle.h"
#include "Vec2.h"

class CollisionConstraint : public Constraint
{
public:
	CollisionConstraint(std::vector<Particle*> particles);

	Vec2 gradient(Particle& particle);

	void calculateError();
};