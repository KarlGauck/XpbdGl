#pragma once

#include "constraint.h"
#include "particle.h"

class DistanceConstraint : public Constraint
{
public:
	DistanceConstraint(std::vector<Particle*> particles, float compliance, float distance);

	Vec2 gradient(Particle& particle);

	void calculateError();
private:
	float distance;
};