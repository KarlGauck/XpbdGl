#pragma once

#include <vector>
#include "instanceData.h"

class Solver 
{
public:

	virtual std::vector<InstanceData> getCircleData()
	{
		return {};
	}

	virtual void solve(float dt) = 0;

	virtual void addParticle(Vec2 pos, Vec2 vel) = 0;

};