#pragma once

#include "constraint.h"
#include "particle.h"
#include "Vec2.h"

class MaxPositionConstraint : public Constraint
{
public:
	MaxPositionConstraint(std::vector<Particle*> particles, float xMax, float yMax, float xMin, float yMin);

	Vec2 gradient(Particle& particle);

	void calculateError();
private:
	float xMax, yMax;
	float xMin, yMin;
};
