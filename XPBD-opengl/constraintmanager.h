#pragma once

#include <vector>

#include "particle.h"
#include "spatialhashgrid.h"
#include "constraint.h"

class ConstraintManager
{
public:
	ConstraintManager(std::vector<Particle*> particles);
	
	void addParticle(Particle* particle);
	virtual std::vector<Constraint*> calculateConstraints() = 0;

protected:
	std::vector<Particle*> particles;
};