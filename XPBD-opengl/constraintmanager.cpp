#include "constraintmanager.h"

ConstraintManager::ConstraintManager(std::vector<Particle*> particles): particles(particles)
{}

void ConstraintManager::addParticle(Particle* particle)
{
	particles.push_back(particle);
}
