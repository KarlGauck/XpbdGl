#include "verletSolver.h"
#include "particle.h"

void VerletSolver::solve(float dt)
{
	Vec2 gravity = Vec2(0.0f, -9.81);
	
	for (Particle& particle : particles)
	{
		Vec2 oldPos = particle.pos;
		particle.pos += (particle.pos - particle.oldPos) + gravity * dt*dt;

		if (particle.pos.y < -10)
			particle.pos.y = -10;
		if (particle.pos.x < -15)
			particle.pos.x = -15;
		if (particle.pos.x > 15)
			particle.pos.x = 15;

		particle.oldPos = oldPos;

		solveCollisions();
	}
}

void VerletSolver::solveCollisions()
{
	int steps = 10;
	for (int step = 0; step < steps; step++)
	{
		for (int pI1 = 0; pI1 < particles.size(); pI1++)
		{
			Particle& p1 = particles[pI1];
			for (int pI2 = 0; pI2 < particles.size(); pI2++)
			{
				Particle& p2 = particles[pI2];
				if (pI1 == pI2)
					continue;
				if (!collide(pI1, pI2))
					continue;
				float error = 1-p2.pos.distance(p1.pos);
				Vec2 delta = p2.pos - p1.pos;
				p1.pos -= delta * .5 * error;
				p2.pos += delta * .5 * error;
			}
		}
	}
}

bool VerletSolver::collide(int particle1, int particle2)
{
	Particle& p1 = particles[particle1];
	Particle& p2 = particles[particle2];
	return p1.pos.distance(p2.pos) < 1;
}

void VerletSolver::addParticle(Vec2 pos, Vec2 vel)
{
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	particles.push_back(
		Particle (
			1.0,
			0.5,
			pos,
			vel,
			color
		)
	);
} 

std::vector<InstanceData> VerletSolver::getCircleData()
{
	std::vector<InstanceData> data;
	for (Particle& particle : particles)
		data.push_back(
			InstanceData{
				particle.pos,
				Vec2(particle.radius * 2, particle.radius * 2),
				Vec2(1.0f, 0.0f),
				{
					particle.color[0],
					particle.color[1],
					particle.color[2],
					particle.color[3],
				}
			}
		);
	return data;
}
