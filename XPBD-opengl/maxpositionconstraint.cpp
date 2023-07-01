#pragma once

#include <vector>
#include <iostream>

#include "Vec2.h"
#include "particle.h"
#include "maxpositionconstraint.h"

MaxPositionConstraint::MaxPositionConstraint(std::vector<Particle*> particles, float xMax, float yMax, float xMin, float yMin) : 
	Constraint(particles, 0.00100001f), xMax(xMax), yMax(yMax), xMin(xMin), yMin(yMin)
{};

void MaxPositionConstraint::calculateError()
{
	Particle& particle = *particles[0];
	float capX = particle.pos.x;
	float capY = particle.pos.y;
	if (particle.pos.x > xMax)
		capX = xMax;
	if (particle.pos.x < xMin)
		capX = xMin;
	if (particle.pos.y > yMax)
		capY = yMax;
	if (particle.pos.y < yMin)
		capY = yMin;
	error = particle.pos.distance(Vec2(capX, capY));
}

Vec2 MaxPositionConstraint::gradient(Particle& particle)
{
	float capX = particle.pos.x, capY = particle.pos.y;

	if (particle.pos.x > xMax)
		capX = xMax;
	if (particle.pos.x < xMin)
		capX = xMin;
	if (particle.pos.y > yMax)
		capY = yMax;
	if (particle.pos.y < yMin)
		capY = yMin;

	return (particle.pos - Vec2(capX, capY)).normalized();
};