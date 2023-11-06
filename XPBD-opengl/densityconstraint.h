#include "Vec2.h"
#include "particle.h"

class Densityconstraint {
public:
	Vec2 calculateGradient(Particle& particle);
	void calculateError();

	Densityconstraint();
};