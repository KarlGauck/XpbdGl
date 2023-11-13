#pragma once

#include <vector>

#include "solver.h"
#include "viewportData.h"

class Scene {
public:
	Solver* solver;
	ViewportData viewportData;

	void step();
	
	virtual std::vector<InstanceData> getCircleData();
	virtual std::vector<InstanceData> getRectData();
	virtual std::vector<LineData> getLineData();
	virtual void mouseDownEvent(Vec2 pos);
};
