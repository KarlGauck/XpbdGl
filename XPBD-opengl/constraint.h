#pragma once

#include <vector>

#include "particle.h"
#include "Vec2.h"

class Constraint {
public:
	Constraint(std::vector<Particle*> particles, float compliance);

	std::vector<Particle*> particles;

	virtual Vec2 gradient(Particle& particle) = 0;
	virtual void calculateError() = 0;

	void calculateLambda(float dt);

	Vec2 deltaPos(Particle& particle);

private:
	float lambda;
	float compliance;
protected:
	float error;
};