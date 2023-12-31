#include "Vec2.h"
#include "particle.h"
#include "spatialhashgrid.h"

#include <vector>

class DensityConstraint {
public:
	const static int ConstraintCountMax = 100000;
	static float KernelRadius;
	static float RestDensity;
	static float EpsilonRelaxation;
	static float DoubleCosKVal;

	static std::vector<DensityConstraint> constraints;

	static void solve(std::vector<Particle>& globalParticles, SpatialHashGrid grid, float dt);
	static void add(int particle, std::vector<int> neighbours);

	int particle;
	std::vector<int> neighbours;

	void calculateGradient(std::vector<Particle>& globalParticles);
	void calculateError(std::vector<Particle>& globalParticles);

private:
	DensityConstraint(int particle, std::vector<int>& neighbours);

	static void calculateDensity(std::vector<Particle>& particles, SpatialHashGrid grid);
	static float Poly6Kernel(float radius);
	static float SpikyKernel(float radius);
	static float Poly6KernelGrad(float radius);
	static float SpikyKernelGrad(float radius);
	static float DoubleCosKernel(float r);
	static float DoubleCosKernelGrad(float r);

	float compliance = 0.000001f;

	Vec2 gradient;
	float error;
	float lambda;
};